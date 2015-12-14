load("/nanopb", "nanopb_cpp_library")

nanopb_cpp_library(
    name = "person",
    proto = "person.proto",
)

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    deps = [":person"],
)

nanopb_cpp_library(
    name = "simple",
    proto = "simple.proto",
)

cc_test(
    name = "simple_test",
    srcs = ["simple_test.cc"],
    deps = [
        ":simple",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
