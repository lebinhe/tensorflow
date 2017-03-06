bazel --output_base /tmp/bazelout --output_user_root /data/li/cache build --config=opt --config=cuda --config=rdma //tensorflow/tools/pip_package:build_pip_package
