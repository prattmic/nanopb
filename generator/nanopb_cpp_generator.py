#!/usr/bin/env python

import argparse
import os.path

import generator.nanopb_generator as nanopb

# TODO: plugin

def generate_class(message):
    name = message.name
    base = "_%s" % name

    print "\nclass %s : %s {" % (name, base)

    # Common code
    print """
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

""".format(name=name, base=base)

    for field in message.fields:
        if field.allocation != 'STATIC' or field.allocation == 'CALLBACK':
            print """#error "Field %s has unsupported allocation '%s'" """ % (field.name, field.allocation)
            continue
        if field.array_decl:
            print """#error "Field %s has unsupported array_decl %s" """ % (field.name, field.array_decl)
            continue

        print """
    {type} get_{name}() {{
        return {name};
    }}

    void set_{name}({type} val) {{
        {name} = val;
    }}
""".format(name=field.name, type=field.ctype)

    print "}"

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate nanopb C++ bindings')
    parser.add_argument('files', nargs='+', help='files to parse')

    args = parser.parse_args()

    for f in args.files:
        options, _ = nanopb.optparser.parse_args([])

        proto = nanopb.parse_file(f, None, options)

        print '#include "%s.pb.h"' % (os.path.splitext(f)[0])

        for m in proto.messages:
            generate_class(m)
