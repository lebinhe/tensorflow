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

#ifndef THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_MASTER_SERVICE_IMPL_H_
#define THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_MASTER_SERVICE_IMPL_H_

#include "Accelio++/impl/codegen/async_stream.h"
#include "Accelio++/impl/codegen/async_unary_call.h"
#include "Accelio++/impl/codegen/proto_utils.h"
#include "Accelio++/impl/codegen/rpc_method.h"
#include "Accelio++/impl/codegen/service_type.h"
#include "Accelio++/impl/codegen/status.h"
#include "Accelio++/impl/codegen/stub_options.h"
#include "Accelio++/impl/codegen/sync_stream.h"

#include "tensorflow/core/distributed_runtime/rpc/Accelio_serialization_traits.h"
#include "tensorflow/core/protobuf/master.pb.h"

// Contains potentially large GraphDef.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::CreateSessionRequest);
// Contains potentially large GraphDef.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::ExtendSessionRequest);
// Contains potentially large TensorProto.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::RunStepRequest);
// Contains potentially large StepStats, TensorProto.
TF_Accelio_ALLOW_UNLIMITED_MESSAGE_SIZE(tensorflow::RunStepResponse);

namespace Accelio {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace Accelio

namespace tensorflow {

namespace Accelio {

// Implementation of `tensorflow.MasterService`, based on the
// definition in "//tensorflow/core/protobuf/master_service.proto",
// and the Accelio generated stub and service classes.
// See that file for the definition of methods and messages.
class MasterService Accelio_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::Accelio::Status CreateSession(::Accelio::ClientContext* context,
                                         const CreateSessionRequest& request,
                                         CreateSessionResponse* response) = 0;
    virtual ::Accelio::Status ExtendSession(::Accelio::ClientContext* context,
                                         const ExtendSessionRequest& request,
                                         ExtendSessionResponse* response) = 0;
    virtual ::Accelio::Status PartialRunSetup(
        ::Accelio::ClientContext* context, const PartialRunSetupRequest& request,
        PartialRunSetupResponse* response) = 0;
    virtual ::Accelio::Status RunStep(::Accelio::ClientContext* context,
                                   const RunStepRequest& request,
                                   RunStepResponse* response) = 0;
    virtual ::Accelio::Status CloseSession(::Accelio::ClientContext* context,
                                        const CloseSessionRequest& request,
                                        CloseSessionResponse* response) = 0;
    virtual ::Accelio::Status ListDevices(::Accelio::ClientContext* context,
                                       const ListDevicesRequest& request,
                                       ListDevicesResponse* response) = 0;
    virtual ::Accelio::Status Reset(::Accelio::ClientContext* context,
                                 const ResetRequest& request,
                                 ResetResponse* response) = 0;
  };
  class Stub Accelio_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::Accelio::ChannelInterface>& channel);
    ::Accelio::Status CreateSession(::Accelio::ClientContext* context,
                                 const CreateSessionRequest& request,
                                 CreateSessionResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status ExtendSession(::Accelio::ClientContext* context,
                                 const ExtendSessionRequest& request,
                                 ExtendSessionResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status PartialRunSetup(
        ::Accelio::ClientContext* context, const PartialRunSetupRequest& request,
        PartialRunSetupResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status RunStep(::Accelio::ClientContext* context,
                           const RunStepRequest& request,
                           RunStepResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status CloseSession(::Accelio::ClientContext* context,
                                const CloseSessionRequest& request,
                                CloseSessionResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status ListDevices(::Accelio::ClientContext* context,
                               const ListDevicesRequest& request,
                               ListDevicesResponse* response) Accelio_OVERRIDE;
    ::Accelio::Status Reset(::Accelio::ClientContext* context,
                         const ResetRequest& request,
                         ResetResponse* response) Accelio_OVERRIDE;

   private:
    std::shared_ptr< ::Accelio::ChannelInterface> channel_;
    const ::Accelio::RpcMethod rpcmethod_CreateSession_;
    const ::Accelio::RpcMethod rpcmethod_ExtendSession_;
    const ::Accelio::RpcMethod rpcmethod_PartialRunSetup_;
    const ::Accelio::RpcMethod rpcmethod_RunStep_;
    const ::Accelio::RpcMethod rpcmethod_CloseSession_;
    const ::Accelio::RpcMethod rpcmethod_ListDevices_;
    const ::Accelio::RpcMethod rpcmethod_Reset_;
  };
  static std::unique_ptr<Stub> NewStub(
      const std::shared_ptr< ::Accelio::ChannelInterface>& channel,
      const ::Accelio::StubOptions& options = ::Accelio::StubOptions());

  class AsyncService : public ::Accelio::Service {
   public:
    AsyncService();
    virtual ~AsyncService();
    void RequestCreateSession(
        ::Accelio::ServerContext* context, CreateSessionRequest* request,
        ::Accelio::ServerAsyncResponseWriter<CreateSessionResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(0, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestExtendSession(
        ::Accelio::ServerContext* context, ExtendSessionRequest* request,
        ::Accelio::ServerAsyncResponseWriter<ExtendSessionResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(1, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestPartialRunSetup(
        ::Accelio::ServerContext* context, PartialRunSetupRequest* request,
        ::Accelio::ServerAsyncResponseWriter<PartialRunSetupResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(2, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestRunStep(
        ::Accelio::ServerContext* context, RunStepRequest* request,
        ::Accelio::ServerAsyncResponseWriter<RunStepResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(3, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestCloseSession(
        ::Accelio::ServerContext* context, CloseSessionRequest* request,
        ::Accelio::ServerAsyncResponseWriter<CloseSessionResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(4, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestListDevices(
        ::Accelio::ServerContext* context, ListDevicesRequest* request,
        ::Accelio::ServerAsyncResponseWriter<ListDevicesResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(5, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
    void RequestReset(
        ::Accelio::ServerContext* context, ResetRequest* request,
        ::Accelio::ServerAsyncResponseWriter<ResetResponse>* response,
        ::Accelio::CompletionQueue* new_call_cq,
        ::Accelio::ServerCompletionQueue* notification_cq, void* tag) {
      ::Accelio::Service::RequestAsyncUnary(6, context, request, response,
                                         new_call_cq, notification_cq, tag);
    }
  };
};

}  // namespace Accelio

}  // namespace tensorflow

#endif  // THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_MASTER_SERVICE_IMPL_H_
