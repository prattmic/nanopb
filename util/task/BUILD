cc_library(
    name = "status",
    srcs = [
        "status.cc",
    ],
    hdrs = [
        "status.h",
        "statusor.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "statusor_test",
    srcs = ["statusor_test.cc"],
    deps = [
        ":status",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
