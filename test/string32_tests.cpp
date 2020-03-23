#include <strn/io.hpp>
#include <strn/string32.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

using namespace strn::literals;

TEST(string32_tests, test_constructor_integer)
{
    strn::string32::uint ival(0x11223344);
    strn::string32 str(ival);
    ASSERT_EQ(str.integer(), ival);
    ASSERT_EQ(str.hash(), static_cast<std::size_t>(ival));
}

TEST(string56_tests, test_s64_literal)
{
    strn::string32::uint ival = "c2E4"_s32.integer();
    ASSERT_EQ(ival, ('c'|('2'<<8)|('E'<<16)|('4'<<24)));

    strn::string32::uint small_ival = "c12"_s32.integer();
    ASSERT_EQ(small_ival, ('c'|('1'<<8)|('2'<<16)));
}

TEST(string32_tests, test_constructor_empty)
{
    strn::string32 str32;
    ASSERT_EQ(str32.integer(), 0);
    ASSERT_EQ(str32.hash(), 0);
}

TEST(string32_tests, test_constructor_std_string_view)
{
    std::string str(".2E4");
    std::string_view strv(str);
    strn::string32 str32(strv);
    ASSERT_EQ(str32.integer(), ".2E4"_s32.integer());
    ASSERT_EQ(str32.hash(), ".2E4"_s32.hash());

    std::string small_str("c12");
    std::string_view small_strv(small_str);
    strn::string32 small_str32(small_strv);
    ASSERT_EQ(small_str32.integer(), "c12"_s32.integer());
    ASSERT_EQ(small_str32.hash(), "c12"_s32.hash());
}

TEST(string32_tests, test_constructor_too_long_std_string_view)
{
    std::string str("12345");
    std::string_view strv(str);
    strn::string32 str32(strv);
    ASSERT_EQ(str32.integer(), "1234"_s32.integer());
    ASSERT_EQ(str32.hash(), "1234"_s32.hash());
}

TEST(string32_tests, test_iterators)
{
    strn::string32 str32("abcd");
    ASSERT_EQ(*str32.begin(), 'a');
    ASSERT_EQ(std::distance(str32.begin(), str32.end()), 4);
    ASSERT_EQ(*std::prev(str32.end()), 'd');

    strn::string32 small_str32("ab");
    ASSERT_EQ(*small_str32.begin(), 'a');
    ASSERT_EQ(std::distance(small_str32.begin(), small_str32.end()), 2);
    ASSERT_EQ(*std::prev(small_str32.end()), 'b');
}

TEST(string32_tests, test_const_iterators)
{
    const strn::string32 const_str32("abcd");
    ASSERT_EQ(*const_str32.begin(), 'a');
    ASSERT_EQ(std::distance(const_str32.begin(), const_str32.end()), 4);
    ASSERT_EQ(*std::prev(const_str32.end()), 'd');

    const strn::string32 const_small_str32("ab");
    ASSERT_EQ(*const_small_str32.begin(), 'a');
    ASSERT_EQ(std::distance(const_small_str32.begin(), const_small_str32.end()), 2);
    ASSERT_EQ(*std::prev(const_small_str32.end()), 'b');

    strn::string32 str32("abcd");
    ASSERT_EQ(*str32.cbegin(), 'a');
    ASSERT_EQ(std::distance(str32.cbegin(), str32.cend()), 4);
    ASSERT_EQ(*std::prev(str32.cend()), 'd');

    strn::string32 small_str32("ab");
    ASSERT_EQ(*small_str32.cbegin(), 'a');
    ASSERT_EQ(std::distance(small_str32.cbegin(), small_str32.cend()), 2);
    ASSERT_EQ(*std::prev(small_str32.cend()), 'b');
}

TEST(string32_tests, test_str_correctness)
{
    strn::string32 str32("east");
    ASSERT_EQ(str32.to_string(), "east");
    ASSERT_NE(str32.integer(), 0);
    ASSERT_NE(str32.hash(), 0);
    ASSERT_EQ(str32.integer(), "east"_s32.integer());
    ASSERT_EQ(str32.hash(), "east"_s32.hash());
}

TEST(string32_tests, test_constructor_std_string)
{
    std::string str("465c");
    strn::string32 str32(str);
    ASSERT_EQ(str32.integer(), "465c"_s32.integer());
    ASSERT_EQ(str32.hash(), "465c"_s32.hash());
}

TEST(string32_tests, test_constructor_c_str_n)
{
    strn::string32 str32("Goal");
    ASSERT_EQ(str32.integer(), "Goal"_s32.integer());

    strn::string32 small_str32("^_^");
    ASSERT_EQ(small_str32.integer(), "^_^"_s32.integer());
}

TEST(string32_tests, test_constructor_c_str)
{
    const char* cstr = "Aze";
    strn::string32 str32(cstr);
    ASSERT_EQ(str32.integer(), "Aze"_s32.integer());
}

TEST(string32_tests, test_operator_eq_and_neq)
{
    strn::string32 str("Goal");
    ASSERT_NE(str, ""_s32);
    ASSERT_NE("Goal"_s32, ""_s32);
    ASSERT_EQ(str, "Goal"_s32);
    strn::string32 str2("Ball");
    ASSERT_NE(str, str2);
}

TEST(string32_tests, test_hash)
{
    strn::string32 str("abcd");

    switch (str.hash())
    {
    case "abcd"_s32.hash():
        SUCCEED();
        break;
    default:
        FAIL() << "hash() method does not work.";
    }
}

TEST(string32_tests, test_to_string_view)
{
    strn::string32 str32("Goal");
    std::string_view strv = str32.to_string_view();
    ASSERT_EQ(strv, "Goal");
    str32 = "read"_s32;
    ASSERT_NE(strv, "Goal");
    ASSERT_EQ(strv, "read");
}

TEST(string32_tests, test_to_string)
{
    strn::string32 str32("Goal");
    std::string str = str32.to_string();
    ASSERT_EQ(str, "Goal");
    str32 = "read"_s32;
    ASSERT_EQ(str, "Goal");
}

TEST(string32_tests, test_empty_or_not)
{
    strn::string32 str32;
    ASSERT_TRUE(str32.empty());
    ASSERT_TRUE(!str32.not_empty());
    str32 = "full"_s32;
    ASSERT_TRUE(!str32.empty());
    ASSERT_TRUE(str32.not_empty());
}

TEST(string32_tests, test_length)
{
    strn::string32 str32;
    ASSERT_EQ(str32.length(), 0);
    str32 = "123"_s32;
    ASSERT_EQ(str32.length(), 3);
    str32 = "1234"_s32;
    ASSERT_EQ(str32.length(), 4);
    static_assert(strn::string32::max_length() == 4);
}

TEST(string32_tests, test_is_printable)
{
    strn::string32 str32("abcd");
    ASSERT_TRUE(str32.is_printable());
    strn::string32 small_str32("abc");
    ASSERT_TRUE(small_str32.is_printable());
    strn::string32 str32_not_printable("ac\1");
    ASSERT_FALSE(str32_not_printable.is_printable());
}

TEST(string32_tests, test_nth)
{
    strn::string32 str32("abc1");
    ASSERT_EQ(str32[2], 'c');
    str32[1] = 'k';
    ASSERT_EQ(str32[1], 'k');
    ASSERT_EQ(str32, "akc1"_s32);
}

TEST(string32_tests, test_operator_less)
{
    strn::string32 str_aaa("aaa");
    strn::string32 str_bb("bb");
    ASSERT_LT(str_bb, str_aaa);
    ASSERT_LT(str_aaa.to_string_view(), str_bb.to_string_view());
}

enum number : uint32_t
{
    ONE = "ONE"_s32.integer(),
    TWO = "TWO"_s32.integer(),
};

enum class color : uint32_t
{
    PINK = "PINK"_s32.integer(),
    BLUE = "BLUE"_s32.integer(),
};

enum class bad_enum32 : uint16_t
{
};

static_assert(strn::is_enum32_v<number>);
static_assert(strn::is_enum32_v<color>);
static_assert(!strn::is_enum32_v<bad_enum32>);

TEST(string32_tests, test_enum_to_string_32)
{
    strn::string32 stra = strn::to_string32(ONE);
    ASSERT_EQ(stra, "ONE"_s32);
    strn::string32 strb = strn::to_string32(color::PINK);
    ASSERT_EQ(strb, "PINK"_s32);
}

TEST(string32_tests, test_enum_to_string)
{
    std::string stra = strn::to_string(ONE);
    ASSERT_EQ(stra, "ONE");
    std::string strb = strn::to_string(color::PINK);
    ASSERT_EQ(strb, "PINK");
}

TEST(string32_tests, test_string_32_to_enum)
{
    strn::string32 stra("ONE");
    ASSERT_EQ(strn::to_enum<number>(stra), ONE);
    strn::string32 strb("PINK");
    ASSERT_EQ(strn::to_enum<color>(strb), color::PINK);
}

TEST(string32_tests, test_std_hash)
{
    std::hash<strn::string32> std_hash;
    ASSERT_EQ(std_hash("ask"_s32), "ask"_s32.hash());
}

TEST(string32_tests, test_operator_read)
{
    std::istringstream stream("abcdefghi\t123");
    strn::string32 str;
    stream >> str;
    ASSERT_EQ(str, "abcd"_s32);
    ASSERT_EQ(str.integer(), "abcd"_s32.integer());

    std::istringstream small_stream("abc\t123");
    strn::string32 small_str;
    small_stream >> small_str;
    ASSERT_EQ(small_str, "abc"_s32);
    ASSERT_EQ(small_str.integer(), "abc"_s32.integer());
}

TEST(string32_tests, test_operator_write)
{
    strn::string32 str("1234");
    std::ostringstream stream;
    stream << str;
    ASSERT_EQ(stream.str(), str.to_string_view());
}

TEST(string32_tests, test_constexpr)
{
    if constexpr ("a"_s32 == "a"_s32)
    {
        ASSERT_EQ("a"_s32, "a"_s32);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
