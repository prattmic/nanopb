#include "external/googletest/googletest/include/gtest/gtest.h"
#include "tests/cpp/simple.pb.hpp"

// Only required fields present by default.
// Default values are 0 if not specified.
// Defaults apply whether or not field is required.
TEST(SimpleTest, Defaults) {
    Simple s;

    // Required
    EXPECT_TRUE(s.present_required_field());
    EXPECT_EQ(false, s.get_required_field());

    // These are all optional.
    EXPECT_FALSE(s.present_optional_field());
    EXPECT_EQ(false, s.get_optional_field());
    EXPECT_FALSE(s.present_int_field());
    EXPECT_EQ(0, s.get_int_field());
    EXPECT_FALSE(s.present_double_field());
    EXPECT_EQ(0, s.get_double_field());
    EXPECT_FALSE(s.present_string_field());
    EXPECT_STREQ("", s.get_string_field());
    EXPECT_FALSE(s.present_enum_field());
    EXPECT_EQ(0, s.get_enum_field());
    EXPECT_FALSE(s.present_submessage_field());

    auto submsg = s.get_submessage_field();

    EXPECT_TRUE(submsg.present_required_default_three_field());
    EXPECT_EQ(3ull, submsg.get_required_default_three_field());
    EXPECT_FALSE(submsg.present_optional_default_three_field());
    EXPECT_EQ(3ull, submsg.get_optional_default_three_field());
}

TEST(SimpleTest, SetGet) {
    Simple s;

    s.set_required_field(true);
    s.set_optional_field(true);
    s.set_int_field(42);
    s.set_double_field(2.71);
    s.set_string_field("hi");
    s.set_enum_field(nanopb::Simple_Enum_TWO);

    Simple_SubMessage ss;
    ss.set_required_default_three_field(5);
    ss.set_optional_default_three_field(5);
    s.set_submessage_field(ss);

    EXPECT_TRUE(s.present_required_field());
    EXPECT_EQ(true, s.get_required_field());

    EXPECT_TRUE(s.present_optional_field());
    EXPECT_EQ(true, s.get_optional_field());

    EXPECT_TRUE(s.present_int_field());
    EXPECT_EQ(42, s.get_int_field());

    EXPECT_TRUE(s.present_double_field());
    EXPECT_DOUBLE_EQ(2.71, s.get_double_field());

    EXPECT_TRUE(s.present_string_field());
    EXPECT_STREQ("hi", s.get_string_field());

    EXPECT_TRUE(s.present_enum_field());
    EXPECT_EQ(nanopb::Simple_Enum_TWO, s.get_enum_field());

    auto submsg = s.get_submessage_field();

    EXPECT_TRUE(submsg.present_required_default_three_field());
    EXPECT_EQ(5ull, submsg.get_required_default_three_field());

    EXPECT_TRUE(submsg.present_optional_default_three_field());
    EXPECT_EQ(5ull, submsg.get_optional_default_three_field());
}

TEST(SimpleTest, Serialize) {
    Simple orig;

    orig.set_required_field(true);
    orig.set_optional_field(true);
    orig.set_int_field(42);
    orig.set_double_field(2.71);
    orig.set_string_field("hi");
    orig.set_enum_field(nanopb::Simple_Enum_TWO);

    Simple_SubMessage submsg;
    submsg.set_required_default_three_field(5);
    submsg.set_optional_default_three_field(5);
    orig.set_submessage_field(submsg);

    uint8_t buf[Simple_size] = { 0 };
    auto serialize_status = orig.Serialize(buf, Simple_size);
    ASSERT_TRUE(serialize_status.ok());
    EXPECT_LE(serialize_status.Value(), static_cast<size_t>(Simple_size));

    auto deserialize_status = Simple::Deserialize(buf, Simple_size);
    ASSERT_TRUE(deserialize_status.ok());

    auto decoded = deserialize_status.Value();

    EXPECT_EQ(orig.get_required_field(), decoded.get_required_field());
    EXPECT_EQ(orig.get_optional_field(), decoded.get_optional_field());
    EXPECT_EQ(orig.get_int_field(), decoded.get_int_field());
    EXPECT_DOUBLE_EQ(orig.get_double_field(), decoded.get_double_field());
    EXPECT_STREQ(orig.get_string_field(), decoded.get_string_field());
    EXPECT_EQ(orig.get_enum_field(), decoded.get_enum_field());
    EXPECT_EQ(orig.get_submessage_field().get_required_default_three_field(),
              decoded.get_submessage_field().get_required_default_three_field());
    EXPECT_EQ(orig.get_submessage_field().get_optional_default_three_field(),
              decoded.get_submessage_field().get_optional_default_three_field());
}
