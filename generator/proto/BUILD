# TODO(prattmic): Real bzl protoc rules.
genrule(
    name = "nanopb_py_src",
    srcs = [
        "nanopb.proto",
        "google/protobuf/descriptor.proto",
    ],
    outs = ["nanopb_pb2.py"],
    cmd = "protoc --python_out=$(@D) -Igenerator/proto $(location :nanopb.proto)",
)

py_library(
    name = "nanopb_py",
    srcs = [":nanopb_py_src"],
    visibility = ["//generator:__pkg__"],
)

genrule(
    name = "plugin_py_src",
    srcs = [
        "plugin.proto",
        "google/protobuf/descriptor.proto",
    ],
    outs = ["plugin_pb2.py"],
    cmd = "protoc --python_out=$(@D) -Igenerator/proto $(location :plugin.proto)",
)

py_library(
    name = "plugin_py",
    srcs = [":plugin_py_src"],
    visibility = ["//generator:__pkg__"],
)
