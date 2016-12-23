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

#include "tensorflow/core/distributed_runtime/rpc/Accelio_remote_master.h"

#include "tensorflow/core/distributed_runtime/call_options.h"
#include "tensorflow/core/distributed_runtime/master_interface.h"
#include "tensorflow/core/distributed_runtime/rpc/Accelio_master_service_impl.h"
#include "tensorflow/core/distributed_runtime/rpc/Accelio_util.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/protobuf/master.pb.h"

namespace tensorflow {

// AccelioRemoteMaster is an implementation of the MasterInterface
// that uses Accelio to talk to the Master service.
class AccelioRemoteMaster : public MasterInterface {
 public:
  explicit AccelioRemoteMaster(SharedAccelioChannelPtr client_channel)
      : stub_(Accelio::MasterService::NewStub(client_channel)) {}

  ~AccelioRemoteMaster() override {}

  Status CreateSession(CallOptions* call_options,
                       const CreateSessionRequest* request,
                       CreateSessionResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->CreateSession(&ctx, *request, response));
  }

  Status ExtendSession(CallOptions* call_options,
                       const ExtendSessionRequest* request,
                       ExtendSessionResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->ExtendSession(&ctx, *request, response));
  }

  Status PartialRunSetup(CallOptions* call_options,
                         const PartialRunSetupRequest* request,
                         PartialRunSetupResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->PartialRunSetup(&ctx, *request, response));
  }

  Status RunStep(CallOptions* call_options, const RunStepRequest* request,
                 RunStepResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->RunStep(&ctx, *request, response));
  }

  Status CloseSession(CallOptions* call_options,
                      const CloseSessionRequest* request,
                      CloseSessionResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->CloseSession(&ctx, *request, response));
  }

  Status ListDevices(CallOptions* call_options,
                     const ListDevicesRequest* request,
                     ListDevicesResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->ListDevices(&ctx, *request, response));
  }

  Status Reset(CallOptions* call_options, const ResetRequest* request,
               ResetResponse* response) override {
    ::Accelio::ClientContext ctx;
    ctx.set_fail_fast(false);
    SetDeadline(&ctx, call_options->GetTimeout());
    return FromAccelioStatus(stub_->Reset(&ctx, *request, response));
  }

 private:
  std::unique_ptr<Accelio::MasterService::Stub> stub_;

  void SetDeadline(::Accelio::ClientContext* ctx, int64 time_in_ms) {
    if (time_in_ms > 0) {
      ctx->set_deadline(gpr_time_from_millis(time_in_ms, GPR_TIMESPAN));
    }
  }
};

MasterInterface* NewAccelioMaster(SharedAccelioChannelPtr channel) {
  return new AccelioRemoteMaster(channel);
}

}  // namespace tensorflow
