#!/usr/bin/env python

import argparse
import os.path
import sys

import generator.nanopb_generator as nanopb

# TODO: plugin

def generate_class(args, message):
    name = message.name
    base = "_%s" % name

    args.output.write("\nclass %s : %s {" % (name, base))

    # Common code
    args.output.write("""
    // Default constructor.
    {name}() {{
        *static_cast<{base}>(this) = {name}_init_default;
    }}

    // Decode from a buffer.
    static {name} Decode(uint8_t *buffer, int size) {{
        {name} msg;
        pb_istream_t stream = pb_istream_from_buffer(buffer, size);
        pb_decode(&stream, {name}_fields, &msg);
        return msg;
    }}

    bool Encode(uint8_t *buffer, int size) {{
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, size);
        return pb_encode(&stream, {name}_fields, this);
    }}

""".format(name=name, base=base))

    for field in message.fields:
        if field.allocation != 'STATIC' or field.allocation == 'CALLBACK':
            args.output.write("""#error "Field %s has unsupported allocation '%s'" """ % (field.name, field.allocation))
            continue
        if field.array_decl:
            args.output.write("""#error "Field %s has unsupported array_decl %s" """ % (field.name, field.array_decl))
            continue

        args.output.write("""
    {type} get_{name}() {{
        return {name};
    }}

    void set_{name}({type} val) {{
        {name} = val;
    }}
""".format(name=field.name, type=field.ctype))

    args.output.write("};")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate nanopb C++ bindings')
    parser.add_argument('file', help='file to parse')
    parser.add_argument('-o', '--output', type=argparse.FileType('w'),
                        default=sys.stdout, help='output file')
    parser.add_argument('--include', default=None,
                        help='base nanopb header include path. defaults to PROTO.pb.h.')

    args = parser.parse_args()
    if args.include is None:
        args.include = "%s.pb.h" % (os.path.splitext(args.file)[0])

    options, _ = nanopb.optparser.parse_args([])

    proto = nanopb.parse_file(args.file, None, options)

    args.output.write('#include "%s"' % (args.include))

    for m in proto.messages:
        generate_class(args, m)
