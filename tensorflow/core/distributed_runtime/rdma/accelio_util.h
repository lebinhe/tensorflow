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

#ifndef THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_UTIL_H_
#define THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_UTIL_H_

#include <memory>

#include "Accelio++/Accelio++.h"
#include "tensorflow/core/lib/core/status.h"

namespace tensorflow {

inline Status FromAccelioStatus(const ::Accelio::Status& s) {
  if (s.ok()) {
    return Status::OK();
  } else {
    return Status(static_cast<tensorflow::error::Code>(s.error_code()),
                  s.error_message());
  }
}

inline ::Accelio::Status ToAccelioStatus(const ::tensorflow::Status& s) {
  if (s.ok()) {
    return ::Accelio::Status::OK;
  } else {
    return ::Accelio::Status(static_cast<::Accelio::StatusCode>(s.code()),
                          s.error_message());
  }
}

typedef std::shared_ptr<::Accelio::Channel> SharedAccelioChannelPtr;

}  // namespace tensorflow

#endif  // THIRD_PARTY_TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_Accelio_UTIL_H_
