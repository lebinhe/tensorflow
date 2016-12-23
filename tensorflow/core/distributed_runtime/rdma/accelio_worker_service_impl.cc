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

#include "tensorflow/core/distributed_runtime/rpc/Accelio_worker_service_impl.h"

#include "Accelio++/impl/codegen/async_stream.h"
#include "Accelio++/impl/codegen/async_unary_call.h"
#include "Accelio++/impl/codegen/channel_interface.h"
#include "Accelio++/impl/codegen/client_unary_call.h"
#include "Accelio++/impl/codegen/method_handler_impl.h"
#include "Accelio++/impl/codegen/rpc_service_method.h"
#include "Accelio++/impl/codegen/service_type.h"
#include "Accelio++/impl/codegen/sync_stream.h"

namespace tensorflow {

const char* AccelioWorkerMethodName(AccelioWorkerMethod id) {
  switch (id) {
    case AccelioWorkerMethod::kGetStatus:
      return "/tensorflow.WorkerService/GetStatus";
    case AccelioWorkerMethod::kRegisterGraph:
      return "/tensorflow.WorkerService/RegisterGraph";
    case AccelioWorkerMethod::kDeregisterGraph:
      return "/tensorflow.WorkerService/DeregisterGraph";
    case AccelioWorkerMethod::kRunGraph:
      return "/tensorflow.WorkerService/RunGraph";
    case AccelioWorkerMethod::kCleanupGraph:
      return "/tensorflow.WorkerService/CleanupGraph";
    case AccelioWorkerMethod::kCleanupAll:
      return "/tensorflow.WorkerService/CleanupAll";
    case AccelioWorkerMethod::kRecvTensor:
      return "/tensorflow.WorkerService/RecvTensor";
    case AccelioWorkerMethod::kLogging:
      return "/tensorflow.WorkerService/Logging";
    case AccelioWorkerMethod::kTracing:
      return "/tensorflow.WorkerService/Tracing";
  }
}

namespace Accelio {

WorkerService::AsyncService::AsyncService() {
  for (int i = 0; i < kAccelioNumWorkerMethods; ++i) {
    AddMethod(new ::Accelio::RpcServiceMethod(
        AccelioWorkerMethodName(static_cast<AccelioWorkerMethod>(i)),
        ::Accelio::RpcMethod::NORMAL_RPC, nullptr));
    ::Accelio::Service::MarkMethodAsync(i);
  }
}

WorkerService::AsyncService::~AsyncService() {}

}  // namespace Accelio

}  // namespace tensorflow
