cc_library(
    name = "nanopb",
    srcs = [
        "pb_common.c",
        "pb_common.h",
        "pb_encode.c",
        "pb_decode.c",
    ],
    hdrs = [
        "pb.h",
        "pb_encode.h",
        "pb_decode.h",
    ],
    # We use C++ features, even though these are .c
    copts = [
        "-x",
        "c++",
        "-std=c++0x",
    ],
    visibility = ["//visibility:public"],
)
