#include <arba/strn/io.hpp>
#include <arba/strn/string32.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

using namespace strn::literals;

TEST(string32_tests, test_s32_literal)
{
    strn::string32::uint ival = "c234"_s32.integer();
    std::size_t expected_val = ('c'|('2'<<8)|('3'<<16)|('4'<<24));
    ASSERT_EQ(ival, expected_val);
}

TEST(string32_tests, test_s32_literal_small)
{
    strn::string32::uint small_ival = "c12"_s32.integer();
    ASSERT_EQ(small_ival, ( 'c'|('1'<<8)|('2'<<16) ));
}

TEST(string32_tests, test_s32_empty_literal)
{
    strn::string32::uint ival = ""_s32.integer();
    ASSERT_EQ(ival, 0);
}

TEST(string32_tests, test_constructor_empty)
{
    strn::string32 str32;
    ASSERT_EQ(str32.integer(), 0);
    ASSERT_EQ(str32.hash(), 0);
    ASSERT_TRUE(str32.empty());
    ASSERT_FALSE(str32.not_empty());
}

TEST(string32_tests, test_constructor_empty_std_string_view)
{
    std::string str("");
    std::string_view strv(str);
    strn::string32 str32(strv);
    ASSERT_EQ(str32.integer(), 0);
    ASSERT_EQ(str32.hash(), 0);
    ASSERT_TRUE(str32.empty());
    ASSERT_FALSE(str32.not_empty());
}

TEST(string32_tests, test_constructor_std_string_view)
{
    std::string str(".234");
    std::string_view strv(str);
    strn::string32 str32(strv);
    ASSERT_EQ(str32[0], '.');
    ASSERT_EQ(str32[1], '2');
    ASSERT_EQ(str32[2], '3');
    ASSERT_EQ(str32[3], '4');
    ASSERT_EQ(str32.integer(), ".234"_s32.integer());
    ASSERT_EQ(str32.hash(), ".234"_s32.hash());

    std::string small_str("c12");
    std::string_view small_strv(small_str);
    strn::string32 small_str32(small_strv);
    ASSERT_EQ(small_str32[0], 'c');
    ASSERT_EQ(small_str32[1], '1');
    ASSERT_EQ(small_str32[2], '2');
    ASSERT_EQ(small_str32[3], '\0');
    ASSERT_EQ(small_str32.integer(), "c12"_s32.integer());
    ASSERT_EQ(small_str32.hash(), "c12"_s32.hash());
}

TEST(string32_tests, test_constructor_too_long_std_string_view)
{
    std::string str("123456789");
    std::string_view strv(str);
    strn::string32 str32(strv);
    ASSERT_EQ(str32[0], '1');
    ASSERT_EQ(str32[1], '2');
    ASSERT_EQ(str32[2], '3');
    ASSERT_EQ(str32[3], '4');
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
    strn::string32 str32("art");
    ASSERT_EQ(str32.to_string(), "art");
    ASSERT_NE(str32.integer(), 0);
    ASSERT_NE(str32.hash(), 0);
    ASSERT_EQ(str32.integer(), "art"_s32.integer());
    ASSERT_EQ(str32.hash(), "art"_s32.hash());
}

TEST(string32_tests, test_constructor_std_string)
{
    std::string str("46c");
    strn::string32 str32(str);
    ASSERT_EQ(str32.integer(), "46c"_s32.integer());
    ASSERT_EQ(str32.hash(), "46c"_s32.hash());
}

TEST(string32_tests, test_constructor_c_str_n)
{
    strn::string32 str32("Oven");
    ASSERT_EQ(str32.integer(), "Oven"_s32.integer());
    ASSERT_TRUE(str32.not_empty());

    strn::string32 small_str32("Car");
    ASSERT_EQ(small_str32.integer(), "Car"_s32.hash());
    ASSERT_TRUE(str32.not_empty());

    strn::string32 empty_str32("");
    ASSERT_EQ(empty_str32.integer(), ""_s32.hash());
    ASSERT_TRUE(empty_str32.empty());
}

TEST(string32_tests, test_constructor_c_str)
{
    const char* cstr = "Tic";
    strn::string32 str32(cstr);
    ASSERT_EQ(str32.integer(), "Tic"_s32.integer());
}

TEST(string32_tests, test_operator_eq_and_neq)
{
    strn::string32 str("art");
    ASSERT_NE(str, ""_s32);
    ASSERT_NE("art"_s32, ""_s32);
    ASSERT_EQ(str, "art"_s32);
    strn::string32 str2("car");
    ASSERT_NE(str, str2);
}

TEST(string32_tests, test_hash)
{
    constexpr strn::string32::uint hash_init_value = 0x11223344;
    enum hash_enum : strn::string32::uint
    {
        hash_value = hash_init_value
    };

    strn::string32 str2(hash_value);
    ASSERT_EQ(str2.integer(), hash_init_value);
    ASSERT_EQ(str2.hash(), static_cast<std::size_t>(hash_init_value));
}

TEST(string32_tests, test_hash_2)
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
    strn::string32 str32("art");
    std::string_view strv = str32.to_string_view();
    ASSERT_EQ(strv, "art");
    str32 = "FRA"_s32;
    ASSERT_NE(strv, "art");
    ASSERT_EQ(strv, "FRA");
}

TEST(string32_tests, test_to_string)
{
    strn::string32 str32("art");
    std::string str = str32.to_string();
    ASSERT_EQ(str, "art");
    str32 = "FR"_s32;
    ASSERT_EQ(str, "art");
}

TEST(string32_tests, test_empty_or_not)
{
    strn::string32 str32;
    ASSERT_TRUE(str32.empty());
    ASSERT_TRUE(!str32.not_empty());
    str32 = "son"_s32;
    ASSERT_TRUE(!str32.empty());
    ASSERT_TRUE(str32.not_empty());
}

TEST(string32_tests, test_length_empty)
{
    strn::string32 str32;
    ASSERT_EQ(str32.length(), 0);
}

TEST(string32_tests, test_length_small)
{
    strn::string32 str32 = "123"_s32;
    ASSERT_EQ(str32.length(), 3);
}

TEST(string32_tests, test_length_max)
{
    strn::string32 str32 = "1234"_s32;
    ASSERT_EQ(str32.length(), 4);
    static_assert(strn::string32::max_length() == 4);
}

TEST(string32_tests, test_is_printable)
{
    strn::string32 str32("abcd");
    ASSERT_TRUE(str32.is_printable());
    strn::string32 small_str32("ab");
    ASSERT_TRUE(small_str32.is_printable());
    strn::string32 str32_not_printable("ab\1");
    ASSERT_FALSE(str32_not_printable.is_printable());
}

TEST(string32_tests, test_nth)
{
    strn::string32 str32("ab1");
    ASSERT_EQ(str32[1], 'b');
    str32[2] = 'k';
    ASSERT_EQ(str32[2], 'k');
    ASSERT_EQ(str32, "abk"_s32);
}

TEST(string32_tests, test_operator_less)
{
    strn::string32 str_aaa("aaa");
    strn::string32 str_bb("bb");
    ASSERT_LT(str_bb, str_aaa);
    ASSERT_LT(str_aaa.to_string_view(), str_bb.to_string_view());
}

TEST(string32_tests, test_push_back)
{
    strn::string32 str("aaa");
    char ch = 'b';
    str.push_back(ch);
    strn::string32 stra("aaa");
    strn::string32 strb("aaab");
    ASSERT_NE(str, stra);
    ASSERT_EQ(str, strb);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string32_tests, test_push_back_2)
{
    strn::string32 str("1234");
    char ch = 'b';
    str.push_back(ch);
    strn::string32 expected_str("1234");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, '4');
}

TEST(string32_tests, test_push_back_3)
{
    strn::string32 str;
    char ch = 'b';
    ASSERT_TRUE(str.empty());
    str.push_back(ch);
    ASSERT_TRUE(str.not_empty());

    strn::string32 expected_str("b");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string32_tests, test_pop_back)
{
    strn::string32 str("aaab");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.not_empty());
    strn::string32 stra("aaa");
    strn::string32 strb("aaab");
    ASSERT_EQ(str, stra);
    ASSERT_NE(str, strb);
    const char& last_ch = *(str.end()-1);
    ASSERT_EQ(last_ch, 'a');
}

TEST(string32_tests, test_pop_back_2)
{
    strn::string32 str("a");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.empty());
}

TEST(string32_tests, test_clear)
{
    strn::string32 str("aaab");
    str.clear();
    strn::string32 strb("aaab");
    ASSERT_TRUE(str.empty());
    ASSERT_NE(str, strb);
}

TEST(string32_tests, test_resize_shorter)
{
    strn::string32 str("abc");
    str.resize(2);
    strn::string32 expected_str("ab");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 2);
}

TEST(string32_tests, test_resize_same)
{
    strn::string32 str("abc");
    str.resize(str.length());
    strn::string32 expected_str("abc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 3);
}

TEST(string32_tests, test_resize_longer)
{
    strn::string32 str("ab");
    str.resize(3, 'c');
    strn::string32 expected_str("abc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 3);
}

TEST(string32_tests, test_resize_too_much)
{
    strn::string32 str("abc");
    str.resize(9, 'c');
    strn::string32 expected_str("abcc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), str.max_length());
}

enum number : uint32_t
{
    ONE = "ONE"_s32.integer(),
    TWO = "TWO"_e32,
};

enum class color : uint32_t
{
    PINK = "PINK"_s32.integer(),
    BLUE = "BLUE"_e32,
};

enum class bad_enum32 : uint16_t
{
};

static_assert(strn::is_enum32_v<number>);
static_assert(strn::is_enum32_v<color>);
static_assert(!strn::is_enum32_v<bad_enum32>);

template <class T>
    requires requires { T(0); } && requires { T(ONE); }
constexpr bool constructible_with_enum_but_not_integer() { return false; }

template <class T>
    requires (!requires { T(0); }) && requires { T(ONE); }
constexpr bool constructible_with_enum_but_not_integer() { return true; }

static_assert(!constructible_with_enum_but_not_integer<int>());
static_assert(constructible_with_enum_but_not_integer<strn::string32>());

TEST(string32_tests, test_enum_to_string_64)
{
    strn::string32 stra(ONE);
    ASSERT_EQ(stra, "ONE"_s32);
    strn::string32 strc = strn::string32(TWO);
    ASSERT_EQ(strc, "TWO"_s32);
    strn::string32 strb = strn::string32(color::PINK);
    ASSERT_EQ(strb, "PINK"_s32);
    strn::string32 strd = strn::string32(color::BLUE);
    ASSERT_EQ(strd, "BLUE"_s32);
}

TEST(string32_tests, test_enum_to_string)
{
    std::string stra = strn::string32(ONE).to_string();
    ASSERT_EQ(stra, "ONE");
    std::string strc = strn::string32(TWO).to_string();
    ASSERT_EQ(strc, "TWO");
    std::string strb = strn::string32(color::PINK).to_string();
    ASSERT_EQ(strb, "PINK");
    std::string strd = strn::string32(color::BLUE).to_string();
    ASSERT_EQ(strd, "BLUE");
}

TEST(string32_tests, test_string_64_to_enum)
{
    strn::string32 stra("ONE");
    ASSERT_EQ(stra.to_enum<number>(), ONE);
    strn::string32 strc("TWO");
    ASSERT_EQ(strc.to_enum<number>(), TWO);
    strn::string32 strb("PINK");
    ASSERT_EQ(strb.to_enum<color>(), color::PINK);
    strn::string32 strd("BLUE");
    ASSERT_EQ(strd.to_enum<color>(), color::BLUE);
}

TEST(string32_tests, test_std_hash)
{
    std::hash<strn::string32> std_hash;
    ASSERT_EQ(std_hash("fine"_s32), "fine"_s32.hash());
}

TEST(string32_tests, test_operator_read)
{
    std::istringstream stream("abcde\t123");
    strn::string32 str;
    stream >> str;
    ASSERT_EQ(str, "abcd"_s32);
    ASSERT_EQ(str.integer(), "abcd"_s32.integer());

    std::istringstream small_stream("ab\t123");
    strn::string32 small_str;
    small_stream >> small_str;
    ASSERT_EQ(small_str, "ab"_s32);
    ASSERT_EQ(small_str.integer(), "ab"_s32.integer());
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

TEST(string32_tests, test_constexpr_fail)
{
    if constexpr ("abcdefghi"_s32 != ""_s32)
    {
        ASSERT_EQ("abcdefghi"_s32, "#BAD"_s32);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

constexpr strn::string32 constexpr_str = "cexp";
