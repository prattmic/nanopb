#include "external/googletest/googletest/include/gtest/gtest.h"
#include "tests/cpp/simple.pb.hpp"

TEST(SimpleTest, SetGet) {
    Simple s;

    s.set_this_is_required(true);
    s.set_this_is_optional(true);
    s.set_an_int(42);
    s.set_a_double(2.71);
    s.set_an_enum(nanopb::Simple_Enum_TWO);

    Simple_SubMessage ss;
    ss.set_default_three(5);
    s.set_submessage(ss);

    EXPECT_EQ(true, s.get_this_is_required());
    EXPECT_EQ(true, s.get_this_is_optional());
    EXPECT_EQ(42, s.get_an_int());
    EXPECT_DOUBLE_EQ(2.71, s.get_a_double());
    EXPECT_EQ(nanopb::Simple_Enum_TWO, s.get_an_enum());
    EXPECT_EQ(5ULL, s.get_submessage().get_default_three());
}
