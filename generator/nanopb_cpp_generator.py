#!/usr/bin/env python

import argparse
import os.path
import sys

import generator.nanopb_generator as nanopb

# TODO: plugin

def generate_class(args, message):
    name = message.name
    base = "nanopb::%s" % name

    args.output.write("class %s : public %s {" % (name, base))

    # Common code
    args.output.write("""
 public:
    using _base = {base};

    // Default constructor.
    {name}() {{
        *static_cast<_base*>(this) = nanopb::{name}_init_default;
    }}

    // Copy constructor from base type.
    {name}(const _base& v) {{
        *this = static_cast<{name}>(v);
    }}

    // Decode from a buffer.
    static {name} Decode(uint8_t *buffer, int size) {{
        {name} msg;
        nanopb::pb_istream_t stream = nanopb::pb_istream_from_buffer(buffer, size);
        nanopb::pb_decode(&stream, nanopb::{name}_fields, &msg);
        return msg;
    }}

    bool Encode(uint8_t *buffer, int size) {{
        nanopb::pb_ostream_t stream = nanopb::pb_ostream_from_buffer(buffer, size);
        return nanopb::pb_encode(&stream, nanopb::{name}_fields, this);
    }}

""".format(name=name, base=base))

    for field in message.fields:
        if field.allocation != 'STATIC' or field.allocation == 'CALLBACK':
            args.output.write("""#warning "Field %s has unsupported allocation '%s'"\n""" % (field.name, field.allocation))
            continue
        if field.array_decl:
            args.output.write("""#warning "Field %s has unsupported array_decl %s"\n""" % (field.name, field.array_decl))
            continue

        field_type = str(field.ctype)
        if field.pbtype in ("ENUM", "UENUM"):
            # We don't redefine these, so they come from nanopb directly.
            field_type = "nanopb::"+field_type

        args.output.write("""
    {type} get_{name}() {{
        return {name};
    }}
""".format(name=field.name, type=field_type))

        if field.pbtype == "MESSAGE":
            args.output.write("""
    void set_{name}({type} val) {{
        {name} = static_cast<{type}::_base>(val);
    }}
""".format(name=field.name, type=field_type))
        else:
            args.output.write("""
    void set_{name}({type} val) {{
        {name} = val;
    }}
""".format(name=field.name, type=field_type))

    args.output.write("};\n\n")

def generate(args, proto):
    args.output.write('''
#include "pb_encode.h"
#include "pb_decode.h"
#include "{include}"

'''.format(include=args.include))

    for m in proto.messages:
        generate_class(args, m)

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

    generate(args, proto)
