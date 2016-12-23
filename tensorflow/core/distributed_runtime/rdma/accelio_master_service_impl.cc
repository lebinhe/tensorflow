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

#include "tensorflow/core/distributed_runtime/rpc/Accelio_master_service_impl.h"

#include "Accelio++/impl/codegen/async_stream.h"
#include "Accelio++/impl/codegen/async_unary_call.h"
#include "Accelio++/impl/codegen/channel_interface.h"
#include "Accelio++/impl/codegen/client_unary_call.h"
#include "Accelio++/impl/codegen/method_handler_impl.h"
#include "Accelio++/impl/codegen/rpc_service_method.h"
#include "Accelio++/impl/codegen/service_type.h"
#include "Accelio++/impl/codegen/sync_stream.h"

namespace tensorflow {

namespace Accelio {

static const char* AccelioMasterService_method_names[] = {
    "/tensorflow.MasterService/CreateSession",
    "/tensorflow.MasterService/ExtendSession",
    "/tensorflow.MasterService/PartialRunSetup",
    "/tensorflow.MasterService/RunStep",
    "/tensorflow.MasterService/CloseSession",
    "/tensorflow.MasterService/ListDevices",
    "/tensorflow.MasterService/Reset",
};

std::unique_ptr<MasterService::Stub> MasterService::NewStub(
    const std::shared_ptr< ::Accelio::ChannelInterface>& channel,
    const ::Accelio::StubOptions& options) {
  std::unique_ptr<MasterService::Stub> stub(new MasterService::Stub(channel));
  return stub;
}

MasterService::Stub::Stub(
    const std::shared_ptr< ::Accelio::ChannelInterface>& channel)
    : channel_(channel),
      rpcmethod_CreateSession_(AccelioMasterService_method_names[0],
                               ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_ExtendSession_(AccelioMasterService_method_names[1],
                               ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_PartialRunSetup_(AccelioMasterService_method_names[2],
                                 ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_RunStep_(AccelioMasterService_method_names[3],
                         ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_CloseSession_(AccelioMasterService_method_names[4],
                              ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_ListDevices_(AccelioMasterService_method_names[5],
                             ::Accelio::RpcMethod::NORMAL_RPC, channel),
      rpcmethod_Reset_(AccelioMasterService_method_names[6],
                       ::Accelio::RpcMethod::NORMAL_RPC, channel) {}

::Accelio::Status MasterService::Stub::CreateSession(
    ::Accelio::ClientContext* context, const CreateSessionRequest& request,
    CreateSessionResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_CreateSession_,
                                   context, request, response);
}

::Accelio::Status MasterService::Stub::ExtendSession(
    ::Accelio::ClientContext* context, const ExtendSessionRequest& request,
    ExtendSessionResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_ExtendSession_,
                                   context, request, response);
}

::Accelio::Status MasterService::Stub::PartialRunSetup(
    ::Accelio::ClientContext* context, const PartialRunSetupRequest& request,
    PartialRunSetupResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_PartialRunSetup_,
                                   context, request, response);
}

::Accelio::Status MasterService::Stub::RunStep(::Accelio::ClientContext* context,
                                            const RunStepRequest& request,
                                            RunStepResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_RunStep_, context,
                                   request, response);
}

::Accelio::Status MasterService::Stub::CloseSession(
    ::Accelio::ClientContext* context, const CloseSessionRequest& request,
    CloseSessionResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_CloseSession_,
                                   context, request, response);
}

::Accelio::Status MasterService::Stub::ListDevices(
    ::Accelio::ClientContext* context, const ListDevicesRequest& request,
    ListDevicesResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_ListDevices_,
                                   context, request, response);
}

::Accelio::Status MasterService::Stub::Reset(::Accelio::ClientContext* context,
                                          const ResetRequest& request,
                                          ResetResponse* response) {
  return ::Accelio::BlockingUnaryCall(channel_.get(), rpcmethod_Reset_, context,
                                   request, response);
}

MasterService::AsyncService::AsyncService() {
  for (int i = 0; i < 7; ++i) {
    AddMethod(new ::Accelio::RpcServiceMethod(AccelioMasterService_method_names[i],
                                           ::Accelio::RpcMethod::NORMAL_RPC,
                                           nullptr));
    ::Accelio::Service::MarkMethodAsync(i);
  }
}

MasterService::AsyncService::~AsyncService() {}

}  // namespace Accelio

}  // namespace tensorflow
