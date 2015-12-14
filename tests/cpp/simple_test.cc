#include "external/googletest/googletest/include/gtest/gtest.h"
#include "tests/cpp/simple.pb.hpp"

TEST(SimpleTest, SetAll) {
    Simple s;

    s.set_this_is_required(true);
    s.set_this_is_optional(true);
    s.set_an_int(42);
    s.set_a_double(2.71);
    s.set_an_enum(nanopb::Simple_Enum_TWO);

    Simple_SubMessage ss;
    ss.set_default_three(5);
    s.set_submessage(ss);
}
