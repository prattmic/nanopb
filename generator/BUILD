py_binary(
    name = "nanopb_generator",
    srcs = ["nanopb_generator.py"],
    deps = [
        "//generator/proto:nanopb_py",
        "//generator/proto:plugin_py",
    ],
)

# nanopb_generator as a library.
py_library(
    name = "nanopb_generator_library",
    srcs = ["nanopb_generator.py"],
    deps = [
        "//generator/proto:nanopb_py",
        "//generator/proto:plugin_py",
    ],
)

py_binary(
    name = "nanopb_cpp_generator",
    srcs = ["nanopb_cpp_generator.py"],
    deps = [
        ":nanopb_generator_library",
    ],
)
