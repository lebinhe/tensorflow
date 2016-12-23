# Description:
#   RDMA communication interfaces and implementations using Accelio for TensorFlow.

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

filegroup(
    name = "all_files",
    srcs = glob(
        ["**/*"],
        exclude = [
            "**/METADATA",
            "**/OWNERS",
        ],
    ),
    visibility = ["//tensorflow:__subpackages__"],
)

filegroup(
    name = "c_srcs",
    data = glob([
        "**/*.cc",
        "**/*.h",
    ]),
)

load(
    "//tensorflow:tensorflow.bzl",
    "tf_cuda_library",
    "tf_cc_test",
)
load("//tensorflow:tensorflow.bzl", "tf_cuda_cc_test")
load("//tensorflow:tensorflow.bzl", "tf_cuda_cc_tests")

# For platform specific build config
load(
    "//tensorflow/core:platform/default/build_config.bzl",
    "tf_kernel_tests_linkstatic",
)
load(
    "//tensorflow/core:platform/default/build_config_root.bzl",
    "tf_cuda_tests_tags",
)

package(default_visibility = [
    "//tensorflow:internal",
])

cc_library(
    name = "Accelio_util",
    srcs = [],
    hdrs = ["Accelio_util.h"],
    deps = [
        "//tensorflow/core:lib",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_client_cq_tag",
    srcs = [],
    hdrs = ["Accelio_client_cq_tag.h"],
    deps = [
        ":Accelio_util",
        "//tensorflow/core:lib",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_remote_worker",
    srcs = ["Accelio_remote_worker.cc"],
    hdrs = ["Accelio_remote_worker.h"],
    deps = [
        ":Accelio_client_cq_tag",
        ":Accelio_util",
        ":Accelio_worker_service_impl",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core:lib_internal",
        "//tensorflow/core:worker_proto_cc",
        "//tensorflow/core/distributed_runtime:worker_cache_logger",
        "//tensorflow/core/distributed_runtime:worker_interface",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_channel",
    srcs = ["Accelio_channel.cc"],
    hdrs = ["Accelio_channel.h"],
    deps = [
        ":Accelio_util",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:lib_internal",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_tensor_coding",
    srcs = ["Accelio_tensor_coding.cc"],
    hdrs = ["Accelio_tensor_coding.h"],
    deps = [
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core:worker_proto_cc",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_call",
    srcs = [],
    hdrs = ["Accelio_call.h"],
    deps = [
        "//tensorflow/core:lib",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "async_service_interface",
    srcs = [],
    hdrs = ["async_service_interface.h"],
    deps = [],
)

cc_library(
    name = "Accelio_worker_cache",
    srcs = ["Accelio_worker_cache.cc"],
    hdrs = ["Accelio_worker_cache.h"],
    deps = [
        ":Accelio_channel",
        ":Accelio_client_cq_tag",
        ":Accelio_remote_worker",
        "//tensorflow/core:lib",
        "//tensorflow/core/distributed_runtime:worker_cache",
        "//tensorflow/core/distributed_runtime:worker_cache_logger",
        "//tensorflow/core/distributed_runtime:worker_cache_partial",
        "//tensorflow/core/distributed_runtime:worker_interface",
    ],
)

tf_cuda_library(
    name = "Accelio_worker_service",
    srcs = ["Accelio_worker_service.cc"],
    hdrs = ["Accelio_worker_service.h"],
    deps = [
        ":async_service_interface",
        ":Accelio_call",
        ":Accelio_tensor_coding",
        ":Accelio_util",
        ":Accelio_worker_service_impl",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:gpu_runtime",
        "//tensorflow/core:lib",
        "//tensorflow/core:lib_internal",
        "//tensorflow/core:worker_proto_cc",
        "//tensorflow/core/distributed_runtime:graph_mgr",
        "//tensorflow/core/distributed_runtime:rendezvous_mgr_interface",
        "//tensorflow/core/distributed_runtime:worker_cache",
        "//tensorflow/core/distributed_runtime:worker_env",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_worker_service_impl",
    srcs = ["Accelio_worker_service_impl.cc"],
    hdrs = ["Accelio_worker_service_impl.h"],
    deps = [
        ":Accelio_serialization_traits",
        "//tensorflow/core:worker_proto_cc",
        "//tensorflow/core/distributed_runtime:worker_interface",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_remote_master",
    srcs = ["Accelio_remote_master.cc"],
    hdrs = ["Accelio_remote_master.h"],
    deps = [
        ":Accelio_master_service_impl",
        ":Accelio_util",
        "//tensorflow/core:lib",
        "//tensorflow/core:master_proto_cc",
        "//tensorflow/core/distributed_runtime:call_options",
        "//tensorflow/core/distributed_runtime:master_interface",
    ],
    alwayslink = 1,
)

cc_library(
    name = "Accelio_master_service",
    srcs = ["Accelio_master_service.cc"],
    hdrs = ["Accelio_master_service.h"],
    deps = [
        ":async_service_interface",
        ":Accelio_call",
        ":Accelio_master_service_impl",
        ":Accelio_util",
        "//tensorflow/core:lib",
        "//tensorflow/core:master_proto_cc",
        "//tensorflow/core/distributed_runtime:master",
        "@Accelio//:Accelio++_unsecure",
    ],
    alwayslink = 1,
)

cc_library(
    name = "Accelio_master_service_impl",
    srcs = ["Accelio_master_service_impl.cc"],
    hdrs = ["Accelio_master_service_impl.h"],
    deps = [
        ":Accelio_serialization_traits",
        "//tensorflow/core:master_proto_cc",
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "Accelio_serialization_traits",
    srcs = [],
    hdrs = ["Accelio_serialization_traits.h"],
    deps = [
        "@Accelio//:Accelio++_unsecure",
    ],
)

cc_library(
    name = "rpc_rendezvous_mgr",
    srcs = ["rpc_rendezvous_mgr.cc"],
    hdrs = ["rpc_rendezvous_mgr.h"],
    deps = [
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core/distributed_runtime:base_rendezvous_mgr",
        "//tensorflow/core/distributed_runtime:worker_cache",
        "//tensorflow/core/distributed_runtime:worker_env",
        "//tensorflow/core/distributed_runtime:worker_interface",
    ],
)

cc_library(
    name = "Accelio_server_lib",
    srcs = ["Accelio_server_lib.cc"],
    hdrs = ["Accelio_server_lib.h"],
    linkstatic = 1,  # Seems to be needed since alwayslink is broken in bazel
    deps = [
        ":async_service_interface",
        ":Accelio_channel",
        ":Accelio_master_service",
        ":Accelio_worker_cache",
        ":Accelio_worker_service",
        ":rpc_rendezvous_mgr",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core/distributed_runtime:graph_mgr",
        "//tensorflow/core/distributed_runtime:local_master",
        "//tensorflow/core/distributed_runtime:master",
        "//tensorflow/core/distributed_runtime:master_env",
        "//tensorflow/core/distributed_runtime:master_session",
        "//tensorflow/core/distributed_runtime:server_lib",
        "//tensorflow/core/distributed_runtime:worker_env",
        "@Accelio//:Accelio++_unsecure",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "Accelio_tensorflow_server",
    srcs = [
        "Accelio_tensorflow_server.cc",
    ],
    deps = [
        ":Accelio_server_lib",
        "//tensorflow/core:all_kernels",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core/distributed_runtime:server_lib",
        "@Accelio//:Accelio++_unsecure",
    ],
)

tf_cuda_library(
    name = "Accelio_testlib_ops",
    testonly = 1,
    srcs = ["Accelio_testlib_ops.cc"],
    linkstatic = 1,  # Seems to be needed since alwayslink is broken in bazel
    deps = [
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "Accelio_testlib_server",
    testonly = 1,
    srcs = [
        "Accelio_testlib_server.cc",
    ],
    deps = [
        ":Accelio_server_lib",
        ":Accelio_testlib_ops",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core/distributed_runtime:server_lib",
        "//tensorflow/core/kernels:constant_op",
        "//tensorflow/core/kernels:cwise_op",
        "//tensorflow/core/kernels:dense_update_ops",
        "//tensorflow/core/kernels:identity_op",
        "//tensorflow/core/kernels:matmul_op",
        "//tensorflow/core/kernels:reduction_ops",
        "//tensorflow/core/kernels:variable_ops",
        "@Accelio//:Accelio++_unsecure",
    ],
)

tf_cuda_library(
    name = "Accelio_testlib",
    testonly = 1,
    srcs = ["Accelio_testlib.cc"],
    hdrs = ["Accelio_testlib.h"],
    data = [
        ":Accelio_testlib_server",
    ],
    deps = [
        ":Accelio_session",
        ":Accelio_testlib_ops",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core:test",
    ],
    alwayslink = 1,
)

cc_library(
    name = "Accelio_session",
    srcs = ["Accelio_session.cc"],
    hdrs = ["Accelio_session.h"],
    deps = [
        ":Accelio_channel",
        ":Accelio_remote_master",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:master_proto_cc",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core/distributed_runtime:call_options",
        "//tensorflow/core/distributed_runtime:local_master",
        "//tensorflow/core/distributed_runtime:master_interface",
    ],
    alwayslink = 1,
)

tf_cuda_cc_tests(
    name = "rpc_tests",
    size = "small",
    srcs = [
        "Accelio_channel_test.cc",
        "rpc_rendezvous_mgr_test.cc",
    ],
    linkopts = select({
        "//tensorflow:darwin": ["-headerpad_max_install_names"],
        "//conditions:default": [],
    }),
    linkstatic = tf_kernel_tests_linkstatic(),
    tags = tf_cuda_tests_tags() + [],
    deps = [
        ":Accelio_channel",
        ":Accelio_server_lib",
        ":Accelio_session",
        ":Accelio_testlib",
        ":rpc_rendezvous_mgr",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:master_proto_cc",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
        "//tensorflow/core:testlib",
        "//tensorflow/core/distributed_runtime:server_lib",
    ],
)

tf_cc_test(
    name = "Accelio_tensor_coding_test",
    size = "small",
    srcs = ["Accelio_tensor_coding_test.cc"],
    deps = [
        ":Accelio_tensor_coding",
        ":Accelio_testlib",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:core_cpu_internal",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
        "//tensorflow/core:testlib",
        "//tensorflow/core:worker_proto_cc",
        "@Accelio//:Accelio++_unsecure",
    ],
)

tf_cuda_cc_test(
    name = "Accelio_session_test",
    size = "medium",
    srcs = ["Accelio_session_test.cc"],
    linkstatic = tf_kernel_tests_linkstatic(),
    tags = tf_cuda_tests_tags() + ["manual"],
    deps = [
        ":Accelio_channel",
        ":Accelio_server_lib",
        ":Accelio_session",
        ":Accelio_testlib",
        ":rpc_rendezvous_mgr",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//tensorflow/core:master_proto_cc",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
        "//tensorflow/core:testlib",
        "//tensorflow/core/distributed_runtime:server_lib",
        "//tensorflow/core/kernels:constant_op",
        "//tensorflow/core/kernels:dense_update_ops",
        "//tensorflow/core/kernels:matmul_op",
        "//tensorflow/core/kernels:variable_ops",
    ],
)
