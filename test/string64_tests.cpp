#include <arba/strn/io.hpp>
#include <arba/strn/string64.hpp>
#include <cstdlib>
#include <gtest/gtest.h>
#include <sstream>

using namespace strn::literals;

TEST(string64_tests, test_s64_literal)
{
    strn::string64::uint ival = "c234S678"_s64.integer();
    std::size_t expected_val = ('c' | ('2' << 8) | ('3' << 16) | ('4' << 24) | (std::size_t('S') << 32)
                                | (std::size_t('6') << 40) | (std::size_t('7') << 48) | (std::size_t('8') << 56));
    ASSERT_EQ(ival, expected_val);
}

TEST(string64_tests, test_s64_literal_small)
{
    strn::string64::uint small_ival = "c12"_s64.integer();
    ASSERT_EQ(small_ival, ('c' | ('1' << 8) | ('2' << 16)));
}

TEST(string64_tests, test_s64_empty_literal)
{
    strn::string64::uint ival = ""_s64.integer();
    ASSERT_EQ(ival, 0);
}

TEST(string64_tests, test_constructor_empty)
{
    strn::string64 str64;
    ASSERT_EQ(str64.integer(), 0);
    ASSERT_EQ(str64.hash(), 0);
    ASSERT_TRUE(str64.empty());
    ASSERT_FALSE(str64.not_empty());
}

TEST(string64_tests, test_constructor_empty_std_string_view)
{
    std::string str("");
    std::string_view strv(str);
    strn::string64 str64(strv);
    ASSERT_EQ(str64.integer(), 0);
    ASSERT_EQ(str64.hash(), 0);
    ASSERT_TRUE(str64.empty());
    ASSERT_FALSE(str64.not_empty());
}

TEST(string64_tests, test_constructor_std_string_view)
{
    std::string str(".234S678");
    std::string_view strv(str);
    strn::string64 str64(strv);
    ASSERT_EQ(str64[0], '.');
    ASSERT_EQ(str64[1], '2');
    ASSERT_EQ(str64[2], '3');
    ASSERT_EQ(str64[3], '4');
    ASSERT_EQ(str64[4], 'S');
    ASSERT_EQ(str64[5], '6');
    ASSERT_EQ(str64[6], '7');
    ASSERT_EQ(str64[7], '8');
    ASSERT_EQ(str64.integer(), ".234S678"_s64.integer());
    ASSERT_EQ(str64.hash(), ".234S678"_s64.hash());

    std::string small_str("c12");
    std::string_view small_strv(small_str);
    strn::string64 small_str64(small_strv);
    ASSERT_EQ(small_str64[0], 'c');
    ASSERT_EQ(small_str64[1], '1');
    ASSERT_EQ(small_str64[2], '2');
    ASSERT_EQ(small_str64[3], '\0');
    ASSERT_EQ(small_str64[4], '\0');
    ASSERT_EQ(small_str64[5], '\0');
    ASSERT_EQ(small_str64[6], '\0');
    ASSERT_EQ(small_str64[7], '\0');
    ASSERT_EQ(small_str64.integer(), "c12"_s64.integer());
    ASSERT_EQ(small_str64.hash(), "c12"_s64.hash());
}

TEST(string64_tests, test_constructor_too_long_std_string_view)
{
    std::string str("123456789");
    std::string_view strv(str);
    strn::string64 str64(strv);
    ASSERT_EQ(str64[0], '1');
    ASSERT_EQ(str64[1], '2');
    ASSERT_EQ(str64[2], '3');
    ASSERT_EQ(str64[3], '4');
    ASSERT_EQ(str64[4], '5');
    ASSERT_EQ(str64[5], '6');
    ASSERT_EQ(str64[6], '7');
    ASSERT_EQ(str64[7], '8');
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
    ASSERT_TRUE(str64.not_empty());

    strn::string64 small_str64("Cactus");
    ASSERT_EQ(small_str64.integer(), "Cactus"_s64.hash());
    ASSERT_TRUE(small_str64.not_empty());

    strn::string64 empty_str64("");
    ASSERT_EQ(empty_str64.integer(), ""_s64.hash());
    ASSERT_TRUE(empty_str64.empty());
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
    constexpr strn::string64::uint hash_init_value = 0x1122334455667788;
    enum hash_enum : strn::string64::uint
    {
        hash_value = hash_init_value
    };

    strn::string64 str2(hash_value);
    ASSERT_EQ(str2.integer(), hash_init_value);
    ASSERT_EQ(str2.hash(), static_cast<std::size_t>(hash_init_value));
}

TEST(string64_tests, test_hash_2)
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

TEST(string64_tests, test_length_empty)
{
    strn::string64 str64;
    ASSERT_EQ(str64.length(), 0);
}

TEST(string64_tests, test_length_small)
{
    strn::string64 str64 = "123"_s64;
    ASSERT_EQ(str64.length(), 3);
}

TEST(string64_tests, test_length_max)
{
    strn::string64 str64 = "12345678"_s64;
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
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string64_tests, test_push_back_2)
{
    strn::string64 str("12345678");
    char ch = 'b';
    str.push_back(ch);
    strn::string64 expected_str("12345678");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, '8');
}

TEST(string64_tests, test_push_back_3)
{
    strn::string64 str;
    char ch = 'b';
    ASSERT_TRUE(str.empty());
    str.push_back(ch);
    ASSERT_TRUE(str.not_empty());

    strn::string64 expected_str("b");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string64_tests, test_pop_back)
{
    strn::string64 str("aaab");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.not_empty());
    strn::string64 stra("aaa");
    strn::string64 strb("aaab");
    ASSERT_EQ(str, stra);
    ASSERT_NE(str, strb);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'a');
}

TEST(string64_tests, test_pop_back_2)
{
    strn::string64 str("a");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.empty());
}

TEST(string64_tests, test_clear)
{
    strn::string64 str("aaab");
    str.clear();
    strn::string64 strb("aaab");
    ASSERT_TRUE(str.empty());
    ASSERT_NE(str, strb);
}

TEST(string64_tests, test_resize_shorter)
{
    strn::string64 str("aabbcc");
    str.resize(4);
    strn::string64 expected_str("aabb");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 4);
}

TEST(string64_tests, test_resize_same)
{
    strn::string64 str("aabbcc");
    str.resize(str.length());
    strn::string64 expected_str("aabbcc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 6);
}

TEST(string64_tests, test_resize_longer)
{
    strn::string64 str("aabb");
    str.resize(6, 'c');
    strn::string64 expected_str("aabbcc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 6);
}

TEST(string64_tests, test_resize_too_much)
{
    strn::string64 str("aabb");
    str.resize(9, 'c');
    strn::string64 expected_str("aabbcccc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), str.max_length());
}

enum number : uint64_t
{
    ONE = "ONE"_s64.integer(),
    TWO = "TWO"_e64,
};

enum class color : uint64_t
{
    ORANGE = "ORANGE"_s64.integer(),
    BLUE = "BLUE"_e64,
};

enum class bad_enum64 : uint32_t
{
};

static_assert(strn::is_enum64_v<number>);
static_assert(strn::is_enum64_v<color>);
static_assert(!strn::is_enum64_v<bad_enum64>);

template <class T>
    requires requires { T(0); } && requires { T(ONE); }
constexpr bool constructible_with_enum_but_not_integer()
{
    return false;
}

template <class T>
    requires(!requires { T(0); }) && requires { T(ONE); }
constexpr bool constructible_with_enum_but_not_integer()
{
    return true;
}

static_assert(!constructible_with_enum_but_not_integer<int>());
static_assert(constructible_with_enum_but_not_integer<strn::string64>());

TEST(string64_tests, test_enum_to_string_64)
{
    strn::string64 stra(ONE);
    ASSERT_EQ(stra, "ONE"_s64);
    strn::string64 strc = strn::string64(TWO);
    ASSERT_EQ(strc, "TWO"_s64);
    strn::string64 strb = strn::string64(color::ORANGE);
    ASSERT_EQ(strb, "ORANGE"_s64);
    strn::string64 strd = strn::string64(color::BLUE);
    ASSERT_EQ(strd, "BLUE"_s64);
}

TEST(string64_tests, test_enum_to_string)
{
    std::string stra = strn::string64(ONE).to_string();
    ASSERT_EQ(stra, "ONE");
    std::string strc = strn::string64(TWO).to_string();
    ASSERT_EQ(strc, "TWO");
    std::string strb = strn::string64(color::ORANGE).to_string();
    ASSERT_EQ(strb, "ORANGE");
    std::string strd = strn::string64(color::BLUE).to_string();
    ASSERT_EQ(strd, "BLUE");
}

TEST(string64_tests, test_string_64_to_enum)
{
    strn::string64 stra("ONE");
    ASSERT_EQ(stra.to_enum<number>(), ONE);
    strn::string64 strc("TWO");
    ASSERT_EQ(strc.to_enum<number>(), TWO);
    strn::string64 strb("ORANGE");
    ASSERT_EQ(strb.to_enum<color>(), color::ORANGE);
    strn::string64 strd("BLUE");
    ASSERT_EQ(strd.to_enum<color>(), color::BLUE);
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

TEST(string64_tests, test_constexpr_fail)
{
    if constexpr ("abcdefghi"_s64 != ""_s64)
    {
        ASSERT_EQ("abcdefghi"_s64, "#BAD_S64"_s64);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

constexpr strn::string64 constexpr_str = "cstexpr";
