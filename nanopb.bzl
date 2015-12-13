def _proto_pb(ctx):
    src = ctx.file.src
    out = ctx.outputs.out

    cmd = "protoc -o %s %s" % (out.path, src.path)

    ctx.action(
        inputs=[src],
        outputs=[out],
        command = cmd,
    )

proto_pb = rule(
    implementation=_proto_pb,
    attrs={
        "src": attr.label(
            mandatory=True,
            allow_files=True,
            single_file=True,
        ),
    },
    outputs={"out": "%{name}.pb"},
)

def _nanopb_srcs(ctx):
    src = ctx.file.src
    _renamed_src = ctx.outputs._renamed_src
    nanopb_generator = ctx.executable._nanopb_generator

    include_dir = ctx.outputs.source.short_path.rsplit("/", 1)[0]

    # nanopb_generator simply creates FOO.pb.h and FOO.pb.c from FOO.pb,
    # so we need to make sure src actually looks like FOO.pb.
    ctx.action(
        inputs=[src],
        outputs=[_renamed_src],
        command = "cp %s %s" % (src.path, _renamed_src.path),
    )

    ctx.action(
        inputs=[_renamed_src],
        outputs=[
            ctx.outputs.source,
            ctx.outputs.header,
        ],
        arguments = [
            "--generated-include-format",
            '#include "%s/%%s"' % (include_dir),
            "--library-include-format",
            '#include "%s"',
            _renamed_src.path
        ],
        executable = nanopb_generator,
    )

nanopb_srcs = rule(
    implementation=_nanopb_srcs,
    attrs={
        "_nanopb_generator": attr.label(
                executable=True,
                default=Label("//generator:nanopb_generator"),
        ),
        "src": attr.label(
            mandatory=True,
            allow_files=True,
            single_file=True,
        ),
    },
    outputs={
        "_renamed_src": "%{name}.pb",
        "source": "%{name}.pb.c",
        "header": "%{name}.pb.h",
    },
    # Required when generating headers.
    output_to_genfiles=True,
)

def nanopb_library(name, src):
    pb_name = name + "_pb"
    srcs_name = name + "_srcs"

    proto_pb(
        name = pb_name,
        src = src,
    )

    nanopb_srcs(
        name = srcs_name,
        src = pb_name,
    )

    native.cc_library(
        name = name,
        srcs = [srcs_name + ".pb.c"],
        hdrs = [srcs_name + ".pb.h"],
        deps = ["//:nanopb"],
    )
