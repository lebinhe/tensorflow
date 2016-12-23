/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/distributed_runtime/rpc/Accelio_channel.h"

#include <limits>
#include <map>
#include <unordered_map>

#include "Accelio++/create_channel.h"

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/gtl/map_util.h"
#include "tensorflow/core/lib/strings/numbers.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/macros.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/util/device_name_utils.h"

namespace tensorflow {

namespace {

string MakeAddress(const string& job, int task) {
  return strings::StrCat("/job:", job, "/replica:0/task:", task);
}

}  // namespace

SharedAccelioChannelPtr NewHostPortAccelioChannel(const string& target) {
  // TODO(mrry): Implement secure channels.
  ::Accelio::ChannelArguments args;
  args.SetInt(Accelio_ARG_MAX_MESSAGE_LENGTH, std::numeric_limits<int32>::max());
  // NOTE(mrry): Some versions of Accelio use a 20-second minimum backoff
  // on connection failure, which makes our tests time out.
  args.SetInt("Accelio.testing.fixed_reconnect_backoff_ms", 1000);
  return ::Accelio::CreateCustomChannel(
      target, ::Accelio::InsecureChannelCredentials(), args);
}

namespace {
Status ValidateHostPortPair(const string& host_port) {
  uint32 port;
  std::vector<string> parts = str_util::Split(host_port, ':');
  // Must be host:port, port must be a number, host must not contain a '/'.
  if (parts.size() != 2 || !strings::safe_strtou32(parts[1], &port) ||
      parts[0].find("/") != string::npos) {
    return errors::InvalidArgument("Could not interpret \"", host_port,
                                   "\" as a host-port pair.");
  }
  return Status::OK();
}
}  // namespace

Status AccelioChannelSpec::AddHostPortsJob(const string& job_id,
                                        const std::vector<string>& host_ports) {
  std::map<int, string> host_ports_map;
  for (size_t i = 0; i < host_ports.size(); ++i) {
    host_ports_map[i] = host_ports[i];
  }
  return AddHostPortsJob(job_id, host_ports_map);
}

Status AccelioChannelSpec::AddHostPortsJob(
    const string& job_id, const std::map<int, string>& host_ports) {
  if (!job_ids_.insert(job_id).second) {
    return errors::InvalidArgument(
        "Duplicate job ID in cluster specification: ", job_id);
  }
  for (const auto& id_host_port : host_ports) {
    TF_RETURN_IF_ERROR(ValidateHostPortPair(id_host_port.second));
  }
  host_ports_jobs_.emplace_back(job_id, host_ports);
  return Status::OK();
}

namespace {

// AccelioChannelCache that caches results to FindWorkerChannel() calls.
class CachingAccelioChannelCache : public AccelioChannelCache {
 public:
  CachingAccelioChannelCache() {}

  ~CachingAccelioChannelCache() override {}

  SharedAccelioChannelPtr FindWorkerChannel(const string& target) override {
    SharedAccelioChannelPtr ch = nullptr;
    {
      mutex_lock l(mu_);  // could use reader lock
      ch = gtl::FindPtrOrNull(channels_, target);
      if (ch) {
        return ch;
      }
    }
    ch = FindChannelOnce(target);
    if (ch) {
      mutex_lock l(mu_);
      channels_.insert({target, ch});
    }
    return ch;
  }

 protected:
  // Find the ClientChannel for "target".  Only called when no channel was
  // found in the channels_ cache for "target".  A non nullptr result will be
  // cached in channels_.
  virtual SharedAccelioChannelPtr FindChannelOnce(const string& target) = 0;

 private:
  // TODO(zhifengc): Eviction when the map becomes too big.
  mutex mu_;
  std::unordered_map<string, SharedAccelioChannelPtr> channels_ GUARDED_BY(mu_);
};

// A ChannelCache that is the union of multiple ChannelCaches.
// Takes ownership of the caches passed to the constructor.
class MultiAccelioChannelCache : public CachingAccelioChannelCache {
 public:
  explicit MultiAccelioChannelCache(const std::vector<AccelioChannelCache*>& caches)
      : CachingAccelioChannelCache(), caches_(caches) {}

  ~MultiAccelioChannelCache() override {
    for (AccelioChannelCache* cache : caches_) {
      delete cache;
    }
  }

  void ListWorkers(std::vector<string>* workers) override {
    for (AccelioChannelCache* cache : caches_) {
      cache->ListWorkers(workers);
    }
  }

  string TranslateTask(const string& target) override {
    mutex_lock l(mu_);  // could use reader lock
    AccelioChannelCache* cache = gtl::FindPtrOrNull(target_caches_, target);
    if (cache == nullptr) {
      for (AccelioChannelCache* c : caches_) {
        string r = c->TranslateTask(target);
        if (!r.empty()) {
          target_caches_.insert({target, c});
          cache = c;
          break;
        }
      }
    }
    CHECK(cache) << "Could not find AccelioChannelCache holding channel for "
                 << target;
    return cache->TranslateTask(target);
  }

 protected:
  SharedAccelioChannelPtr FindChannelOnce(const string& target) override {
    for (AccelioChannelCache* cache : caches_) {
      SharedAccelioChannelPtr ch(cache->FindWorkerChannel(target));
      if (ch) {
        mutex_lock l(mu_);
        target_caches_.insert({target, cache});
        return ch;
      }
    }
    return nullptr;
  }

 private:
  // List of channels used by this MultiAccelioChannelCache.
  const std::vector<AccelioChannelCache*> caches_;

  mutex mu_;
  // Cache of channels keyed by the target they are handling.
  // The same AccelioChannelCache can appear multiple times in the cache.
  std::unordered_map<string, AccelioChannelCache*> target_caches_ GUARDED_BY(mu_);
};

class SparseAccelioChannelCache : public CachingAccelioChannelCache {
 public:
  SparseAccelioChannelCache(const string& job_id,
                         const std::map<int, string>& host_ports,
                         ChannelCreationFunction channel_func)
      : job_id_(job_id),
        host_ports_(host_ports),
        channel_func_(std::move(channel_func)) {
    LOG(INFO) << "Initialize AccelioChannelCache for job " << ToString();
  }
  ~SparseAccelioChannelCache() override {}

  void ListWorkers(std::vector<string>* workers) override {
    workers->reserve(workers->size() + host_ports_.size());
    for (const auto& id_host_port : host_ports_) {
      workers->emplace_back(MakeAddress(job_id_, id_host_port.first));
    }
  }

  string TranslateTask(const string& target) override {
    DeviceNameUtils::ParsedName parsed;
    if (!DeviceNameUtils::ParseFullName(target, &parsed)) {
      LOG(WARNING) << "Invalid target: " << target;
      return "";
    }

    if (!parsed.has_job || parsed.job != job_id_) {
      return "";
    }
    if (!parsed.has_replica || parsed.replica != 0) {
      LOG(WARNING) << "Replica ID must be 0 in target: " << target;
      return "";
    }
    int32 task = parsed.has_task ? parsed.task : -1;
    auto iter = host_ports_.find(task);
    if (iter == host_ports_.end()) {
      LOG(WARNING) << "Task " << task << " was not defined in sparse job "
                   << job_id_ << ": " << target;
      return "";
    }
    return iter->second;
  }

 protected:
  SharedAccelioChannelPtr FindChannelOnce(const string& target) override {
    const string host_port = TranslateTask(target);
    if (host_port.empty()) {
      return nullptr;
    }
    return channel_func_(host_port);
  }

 private:
  string ToString() {
    std::vector<string> task_strings;
    task_strings.reserve(host_ports_.size());
    for (const auto& id_host_port : host_ports_) {
      task_strings.emplace_back(
          strings::StrCat(id_host_port.first, " -> ", id_host_port.second));
    }
    return strings::StrCat(job_id_, " -> {", str_util::Join(task_strings, ", "),
                           "}");
  }

  const string job_id_;
  const std::map<int, string> host_ports_;
  const ChannelCreationFunction channel_func_;
  TF_DISALLOW_COPY_AND_ASSIGN(SparseAccelioChannelCache);
};

}  // namespace

AccelioChannelCache* NewAccelioChannelCache(const AccelioChannelSpec& spec,
                                      ChannelCreationFunction channel_func) {
  const int num_jobs = spec.host_ports_jobs().size();
  if (!num_jobs) {
    LOG(ERROR) << "Empty channel spec.";
    return nullptr;
  }
  std::vector<AccelioChannelCache*> caches;
  caches.reserve(num_jobs);
  for (auto& job : spec.host_ports_jobs()) {
    caches.push_back(
        new SparseAccelioChannelCache(job.job_id, job.host_ports, channel_func));
  }
  return caches.size() == 1 ? caches[0] : new MultiAccelioChannelCache(caches);
}

}  // end namespace tensorflow
