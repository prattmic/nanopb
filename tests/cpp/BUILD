load("/nanopb", "nanopb_cpp_library")

nanopb_cpp_library(
    name = "person",
    src = "person.proto",
)

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    deps = [":person"],
)
