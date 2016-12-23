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

#ifndef THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_WORKER_SERVICE_IMPL_H_
#define THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_WORKER_SERVICE_IMPL_H_

#include "Accelio++/impl/codegen/async_stream.h"
#include "Accelio++/impl/codegen/async_unary_call.h"
#include "Accelio++/impl/codegen/proto_utils.h"
#include "Accelio++/impl/codegen/rpc_method.h"
#include "Accelio++/impl/codegen/service_type.h"
#include "Accelio++/impl/codegen/status.h"
#include "Accelio++/impl/codegen/stub_options.h"
#include "Accelio++/impl/codegen/sync_stream.h"
#include "Accelio++/support/byte_buffer.h"

#include "tensorflow/core/distributed_runtime/rpc/Accelio_serialization_traits.h"
#include "tensorflow/core/distributed_runtime/tensor_coding.h"
#include "tensorflow/core/protobuf/worker.pb.h"

// Contains potentially large GraphDef.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::RegisterGraphRequest);
// Contains potentially large TensorProto.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::RunGraphRequest);
// Contains potentially large StepStats, TensorProto.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::RunGraphResponse);

namespace tensorflow {
class AccelioByteSource : public TensorResponse::Source {
 public:
  explicit AccelioByteSource(Accelio_byte_buffer* buffer) : buffer_(buffer) {}
  ~AccelioByteSource() override { DeleteStream(); }

  typedef ::Accelio::tensorflow_helper::AccelioBufferReader Reader;

  protobuf::io::ZeroCopyInputStream* contents() override {
    DeleteStream();
    stream_ = new (&space_) Reader(buffer_);
    return stream_;
  }

 private:
  void DeleteStream() {
    if (stream_) {
      stream_->~Reader();
    }
  }

  Accelio_byte_buffer* buffer_;  // Not owned
  Reader* stream_ = nullptr;  // Points into space_ if non-nullptr
  char space_[sizeof(Reader)];
};
}  // namespace tensorflow

namespace Accelio {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;

// Support parsing/unparsing of tensorflow::TensorResponse.
// Wire-format is identical to RecvTensorResponse.
template <>
class SerializationTraits<tensorflow::TensorResponse>
    : public UnlimitedSizeProtoSerializationTraits<tensorflow::TensorResponse> {
 public:
  static Status Serialize(const tensorflow::TensorResponse& msg,
                          Accelio_byte_buffer** bp, bool* own_buffer) {
    LOG(FATAL) << "TODO(sanjay,jeff): Implement";
    return Status();
  }
  static Status Deserialize(Accelio_byte_buffer* buffer,
                            tensorflow::TensorResponse* msg,
                            int max_message_size) {
    if (buffer == nullptr) {
      return Status(StatusCode::INTERNAL, "No payload");
    }
    Status result = g_core_codegen_interface->ok();
    if (result.ok()) {
      ::tensorflow::AccelioByteSource source(buffer);
      auto s = msg->ParseFrom(&source);
      if (!s.ok()) {
        result = Status(StatusCode::INTERNAL,
                        ::tensorflow::strings::StrCat(
                            "TensorResponse parse error", s.ToString()));
      }
    }
    g_core_codegen_interface->Accelio_byte_buffer_destroy(buffer);
    return result;
  }
};
}  // namespace Accelio

namespace tensorflow {

// Names of worker methods.
enum class AccelioWorkerMethod {
  kGetStatus,
  kRegisterGraph,
  kDeregisterGraph,
  kRunGraph,
  kCleanupGraph,
  kCleanupAll,
  kRecvTensor,
  kLogging,
  kTracing,
};
static const int kAccelioNumWorkerMethods =
    static_cast<int>(AccelioWorkerMethod::kTracing) + 1;

const char* AccelioWorkerMethodName(AccelioWorkerMethod id);

namespace Accelio {

// Implementation of `tensorflow.WorkerService`, based on the
// definition in "//tensorflow/core/protobuf/worker_service.proto",
// and the Accelio generated stub and service classes.
// See the proto file for the definition of methods and messages.
class WorkerService Accelio_FINAL {
 public:
  class AsyncService : public ::Accelio::Service {
   public:
    AsyncService();
    virtual ~AsyncService();

    // Make RequestAsyncUnary public for Accelio_call.h
    using ::Accelio::Service::RequestAsyncUnary;
  };
};

}  // namespace Accelio

}  // namespace tensorflow

#endif  // THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_WORKER_SERVICE_IMPL_H_
