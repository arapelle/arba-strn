#include <strn/io.hpp>
#include <strn/string64.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

using namespace strn::literals;

TEST(string64_tests, test_constructor_integer)
{
    strn::string64::uint ival(0x1122334455667788);
    strn::string64 str(ival);
    ASSERT_EQ(str.integer(), ival);
    ASSERT_EQ(str.hash(), static_cast<std::size_t>(ival));
}

TEST(string64_tests, test_s64_literal)
{
    strn::string64::uint ival = "c234S678"_s64.integer();
    ASSERT_EQ(ival, ('c'|('2'<<8)|('3'<<16)|('4'<<24)|(std::size_t('S')<<32)|(std::size_t('6')<<40)|(std::size_t('7')<<48)|(std::size_t('8')<<56)));

    strn::string64::uint small_ival = "c12"_s64.integer();
    ASSERT_EQ(small_ival, ('c'|('1'<<8)|('2'<<16)));
}

TEST(string64_tests, test_constructor_empty)
{
    strn::string64 str64;
    ASSERT_EQ(str64.integer(), 0);
    ASSERT_EQ(str64.hash(), 0);
}

TEST(string64_tests, test_constructor_std_string_view)
{
    std::string str(".234S678");
    std::string_view strv(str);
    strn::string64 str64(strv);
    ASSERT_EQ(str64.integer(), ".234S678"_s64.integer());
    ASSERT_EQ(str64.hash(), ".234S678"_s64.hash());

    std::string small_str("c12");
    std::string_view small_strv(small_str);
    strn::string64 small_str64(small_strv);
    ASSERT_EQ(small_str64.integer(), "c12"_s64.integer());
    ASSERT_EQ(small_str64.hash(), "c12"_s64.hash());
}

TEST(string64_tests, test_constructor_too_long_std_string_view)
{
    std::string str("123456789");
    std::string_view strv(str);
    strn::string64 str64(strv);
    ASSERT_EQ(str64.integer(), "12345678"_s64.integer());
    ASSERT_EQ(str64.hash(), "12345678"_s64.hash());
}

TEST(string64_tests, test_iterators)
{
    strn::string64 str64("abcdefgh");
    ASSERT_EQ(*str64.begin(), 'a');
    ASSERT_EQ(std::distance(str64.begin(), str64.end()), 8);
    ASSERT_EQ(*std::prev(str64.end()), 'h');

    strn::string64 small_str64("abcd");
    ASSERT_EQ(*small_str64.begin(), 'a');
    ASSERT_EQ(std::distance(small_str64.begin(), small_str64.end()), 4);
    ASSERT_EQ(*std::prev(small_str64.end()), 'd');
}

TEST(string64_tests, test_const_iterators)
{
    const strn::string64 const_str64("abcdefgh");
    ASSERT_EQ(*const_str64.begin(), 'a');
    ASSERT_EQ(std::distance(const_str64.begin(), const_str64.end()), 8);
    ASSERT_EQ(*std::prev(const_str64.end()), 'h');

    const strn::string64 const_small_str64("abcd");
    ASSERT_EQ(*const_small_str64.begin(), 'a');
    ASSERT_EQ(std::distance(const_small_str64.begin(), const_small_str64.end()), 4);
    ASSERT_EQ(*std::prev(const_small_str64.end()), 'd');

    strn::string64 str64("abcdefgh");
    ASSERT_EQ(*str64.cbegin(), 'a');
    ASSERT_EQ(std::distance(str64.cbegin(), str64.cend()), 8);
    ASSERT_EQ(*std::prev(str64.cend()), 'h');

    strn::string64 small_str64("abcd");
    ASSERT_EQ(*small_str64.cbegin(), 'a');
    ASSERT_EQ(std::distance(small_str64.cbegin(), small_str64.cend()), 4);
    ASSERT_EQ(*std::prev(small_str64.cend()), 'd');
}

TEST(string64_tests, test_str_correctness)
{
    strn::string64 str64("orange");
    ASSERT_EQ(str64.to_string(), "orange");
    ASSERT_NE(str64.integer(), 0);
    ASSERT_NE(str64.hash(), 0);
    ASSERT_EQ(str64.integer(), "orange"_s64.integer());
    ASSERT_EQ(str64.hash(), "orange"_s64.hash());
}

TEST(string64_tests, test_constructor_std_string)
{
    std::string str("46578c");
    strn::string64 str64(str);
    ASSERT_EQ(str64.integer(), "46578c"_s64.integer());
    ASSERT_EQ(str64.hash(), "46578c"_s64.hash());
}

TEST(string64_tests, test_constructor_c_str_n)
{
    strn::string64 str64("Octogone");
    ASSERT_EQ(str64.integer(), "Octogone"_s64.integer());

    strn::string64 small_str64("Cactus");
    ASSERT_EQ(small_str64.integer(), "Cactus"_s64.hash());
}

TEST(string64_tests, test_constructor_c_str)
{
    const char* cstr = "Azerty";
    strn::string64 str64(cstr);
    ASSERT_EQ(str64.integer(), "Azerty"_s64.integer());
}

TEST(string64_tests, test_operator_eq_and_neq)
{
    strn::string64 str("orange");
    ASSERT_NE(str, ""_s64);
    ASSERT_NE("orange"_s64, ""_s64);
    ASSERT_EQ(str, "orange"_s64);
    strn::string64 str2("pomme");
    ASSERT_NE(str, str2);
}

TEST(string64_tests, test_hash)
{
    strn::string64 str("abcdefgh");

    switch (str.hash())
    {
    case "abcdefgh"_s64.hash():
        SUCCEED();
        break;
    default:
        FAIL() << "hash() method does not work.";
    }
}

TEST(string64_tests, test_to_string_view)
{
    strn::string64 str64("orange");
    std::string_view strv = str64.to_string_view();
    ASSERT_EQ(strv, "orange");
    str64 = "France"_s64;
    ASSERT_NE(strv, "orange");
    ASSERT_EQ(strv, "France");
}

TEST(string64_tests, test_to_string)
{
    strn::string64 str64("orange");
    std::string str = str64.to_string();
    ASSERT_EQ(str, "orange");
    str64 = "France"_s64;
    ASSERT_EQ(str, "orange");
}

TEST(string64_tests, test_empty_or_not)
{
    strn::string64 str64;
    ASSERT_TRUE(str64.empty());
    ASSERT_TRUE(!str64.not_empty());
    str64 = "full"_s64;
    ASSERT_TRUE(!str64.empty());
    ASSERT_TRUE(str64.not_empty());
}

TEST(string64_tests, test_length)
{
    strn::string64 str64;
    ASSERT_EQ(str64.length(), 0);
    str64 = "123"_s64;
    ASSERT_EQ(str64.length(), 3);
    str64 = "12345678"_s64;
    ASSERT_EQ(str64.length(), 8);
    static_assert(strn::string64::max_length() == 8);
}

TEST(string64_tests, test_is_printable)
{
    strn::string64 str64("abcdefgh");
    ASSERT_TRUE(str64.is_printable());
    strn::string64 small_str64("abcd");
    ASSERT_TRUE(small_str64.is_printable());
    strn::string64 str64_not_printable("abc\1\2");
    ASSERT_FALSE(str64_not_printable.is_printable());
}

TEST(string64_tests, test_nth)
{
    strn::string64 str64("abc123d");
    ASSERT_EQ(str64[4], '2');
    str64[3] = 'k';
    ASSERT_EQ(str64[3], 'k');
    ASSERT_EQ(str64, "abck23d"_s64);
}

TEST(string64_tests, test_operator_less)
{
    strn::string64 str_aaa("aaa");
    strn::string64 str_bb("bb");
    ASSERT_LT(str_bb, str_aaa);
    ASSERT_LT(str_aaa.to_string_view(), str_bb.to_string_view());
}

TEST(string64_tests, test_push_back)
{
    strn::string64 str("aaa");
    char ch = 'b';
    str.push_back(ch);
    strn::string64 stra("aaa");
    strn::string64 strb("aaab");
    ASSERT_NE(str, stra);
    ASSERT_EQ(str, strb);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string64_tests, test_push_back_2)
{
    strn::string64 str("12345678");
    char ch = 'b';
    str.push_back(ch);
    strn::string64 expected_str("12345678");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, '8');
}

enum number : uint64_t
{
    ONE = "ONE"_s64.integer(),
    TWO = "TWO"_s64.integer(),
};

enum class color : uint64_t
{
    ORANGE = "ORANGE"_s64.integer(),
    BLUE = "BLUE"_s64.integer(),
};

enum class bad_enum64 : uint32_t
{
};

static_assert(strn::is_enum64_v<number>);
static_assert(strn::is_enum64_v<color>);
static_assert(!strn::is_enum64_v<bad_enum64>);

TEST(string64_tests, test_enum_to_string_64)
{
    strn::string64 stra = strn::enum64_to_string64(ONE);
    ASSERT_EQ(stra, "ONE"_s64);
    strn::string64 strb = strn::enum64_to_string64(color::ORANGE);
    ASSERT_EQ(strb, "ORANGE"_s64);
}

TEST(string64_tests, test_enum_to_string)
{
    std::string stra = strn::enum64_to_string(ONE);
    ASSERT_EQ(stra, "ONE");
    std::string strb = strn::enum64_to_string(color::ORANGE);
    ASSERT_EQ(strb, "ORANGE");
}

TEST(string64_tests, test_string_64_to_enum)
{
    strn::string64 stra("ONE");
    ASSERT_EQ(strn::string64_to_enum<number>(stra), ONE);
    strn::string64 strb("ORANGE");
    ASSERT_EQ(strn::string64_to_enum<color>(strb), color::ORANGE);
}

TEST(string64_tests, test_std_hash)
{
    std::hash<strn::string64> std_hash;
    ASSERT_EQ(std_hash("final"_s64), "final"_s64.hash());
}

TEST(string64_tests, test_operator_read)
{
    std::istringstream stream("abcdefghi\t123");
    strn::string64 str;
    stream >> str;
    ASSERT_EQ(str, "abcdefgh"_s64);
    ASSERT_EQ(str.integer(), "abcdefgh"_s64.integer());

    std::istringstream small_stream("abc\t123");
    strn::string64 small_str;
    small_stream >> small_str;
    ASSERT_EQ(small_str, "abc"_s64);
    ASSERT_EQ(small_str.integer(), "abc"_s64.integer());
}

TEST(string64_tests, test_operator_write)
{
    strn::string64 str("12345678");
    std::ostringstream stream;
    stream << str;
    ASSERT_EQ(stream.str(), str.to_string_view());
}

TEST(string64_tests, test_constexpr)
{
    if constexpr ("a"_s64 == "a"_s64)
    {
        ASSERT_EQ("a"_s64, "a"_s64);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
