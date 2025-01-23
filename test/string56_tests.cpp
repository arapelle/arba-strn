#include <arba/strn/io.hpp>
#include <arba/strn/string56.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <sstream>

using namespace strn::literals;

TEST(string56_tests, test_s56_literal)
{
    strn::string56::uint ival = "c234S67"_s56.integer();
    std::size_t expected_val = ('c' | ('2' << 8) | ('3' << 16) | ('4' << 24) | (std::size_t('S') << 32)
                                | (std::size_t('6') << 40) | (std::size_t('7') << 48) | (std::size_t(7) << 56));
    ASSERT_EQ(ival, expected_val);
}

TEST(string56_tests, test_s56_literal_small)
{
    strn::string56::uint small_ival = "c12"_s56.integer();
    ASSERT_EQ(small_ival, ('c' | ('1' << 8) | ('2' << 16) | (std::size_t(3) << 56)));
}

TEST(string56_tests, test_s56_empty_literal)
{
    strn::string56::uint ival = ""_s56.integer();
    ASSERT_EQ(ival, 0);
}

TEST(string56_tests, test_constructor_empty)
{
    strn::string56 str56;
    ASSERT_EQ(str56.integer(), 0);
    ASSERT_EQ(str56.hash(), 0);
    ASSERT_TRUE(str56.empty());
    ASSERT_FALSE(str56.not_empty());
}

TEST(string56_tests, test_constructor_empty_std_string_view)
{
    std::string str("");
    std::string_view strv(str);
    strn::string56 str56(strv);
    ASSERT_EQ(str56.integer(), 0);
    ASSERT_EQ(str56.hash(), 0);
    ASSERT_TRUE(str56.empty());
    ASSERT_FALSE(str56.not_empty());
}

TEST(string56_tests, test_constructor_std_string_view)
{
    std::string str(".234S67");
    std::string_view strv(str);
    strn::string56 str56(strv);
    ASSERT_EQ(str56[0], '.');
    ASSERT_EQ(str56[1], '2');
    ASSERT_EQ(str56[2], '3');
    ASSERT_EQ(str56[3], '4');
    ASSERT_EQ(str56[4], 'S');
    ASSERT_EQ(str56[5], '6');
    ASSERT_EQ(str56[6], '7');
    ASSERT_EQ(str56.integer(), ".234S67"_s56.integer());
    ASSERT_EQ(str56.hash(), ".234S67"_s56.hash());

    std::string small_str("c12");
    std::string_view small_strv(small_str);
    strn::string56 small_str56(small_strv);
    ASSERT_EQ(small_str56[0], 'c');
    ASSERT_EQ(small_str56[1], '1');
    ASSERT_EQ(small_str56[2], '2');
    ASSERT_EQ(small_str56[3], '\0');
    ASSERT_EQ(small_str56[4], '\0');
    ASSERT_EQ(small_str56[5], '\0');
    ASSERT_EQ(small_str56[6], '\0');
    ASSERT_EQ(small_str56.integer(), "c12"_s56.integer());
    ASSERT_EQ(small_str56.hash(), "c12"_s56.hash());
}

TEST(string56_tests, test_constructor_too_long_std_string_view)
{
    std::string str("123456789");
    std::string_view strv(str);
    strn::string56 str56(strv);
    ASSERT_EQ(str56[0], '1');
    ASSERT_EQ(str56[1], '2');
    ASSERT_EQ(str56[2], '3');
    ASSERT_EQ(str56[3], '4');
    ASSERT_EQ(str56[4], '5');
    ASSERT_EQ(str56[5], '6');
    ASSERT_EQ(str56[6], '7');
    ASSERT_EQ(str56.integer(), "1234567"_s56.integer());
    ASSERT_EQ(str56.hash(), "1234567"_s56.hash());
}

TEST(string56_tests, test_iterators)
{
    strn::string56 str56("abcdef");
    ASSERT_EQ(*str56.begin(), 'a');
    ASSERT_EQ(std::distance(str56.begin(), str56.end()), 6);
    ASSERT_EQ(*std::prev(str56.end()), 'f');

    strn::string56 small_str56("abcd");
    ASSERT_EQ(*small_str56.begin(), 'a');
    ASSERT_EQ(std::distance(small_str56.begin(), small_str56.end()), 4);
    ASSERT_EQ(*std::prev(small_str56.end()), 'd');
}

TEST(string56_tests, test_const_iterators)
{
    const strn::string56 const_str56("abcdef");
    ASSERT_EQ(*const_str56.begin(), 'a');
    ASSERT_EQ(std::distance(const_str56.begin(), const_str56.end()), 6);
    ASSERT_EQ(*std::prev(const_str56.end()), 'f');

    const strn::string56 const_small_str56("abcd");
    ASSERT_EQ(*const_small_str56.begin(), 'a');
    ASSERT_EQ(std::distance(const_small_str56.begin(), const_small_str56.end()), 4);
    ASSERT_EQ(*std::prev(const_small_str56.end()), 'd');

    strn::string56 str56("abcdefg");
    ASSERT_EQ(*str56.cbegin(), 'a');
    ASSERT_EQ(std::distance(str56.cbegin(), str56.cend()), 7);
    ASSERT_EQ(*std::prev(str56.cend()), 'g');

    strn::string56 small_str56("abcd");
    ASSERT_EQ(*small_str56.cbegin(), 'a');
    ASSERT_EQ(std::distance(small_str56.cbegin(), small_str56.cend()), 4);
    ASSERT_EQ(*std::prev(small_str56.cend()), 'd');
}

TEST(string56_tests, test_str_correctness)
{
    strn::string56 str56("orange");
    ASSERT_EQ(str56.to_string(), "orange");
    ASSERT_NE(str56.integer(), 0);
    ASSERT_NE(str56.hash(), 0);
    ASSERT_EQ(str56.integer(), "orange"_s56.integer());
    ASSERT_EQ(str56.hash(), "orange"_s56.hash());
}

TEST(string56_tests, test_constructor_std_string)
{
    std::string str("46578c");
    strn::string56 str56(str);
    ASSERT_EQ(str56.integer(), "46578c"_s56.integer());
    ASSERT_EQ(str56.hash(), "46578c"_s56.hash());
}

TEST(string56_tests, test_constructor_c_str_n)
{
    strn::string56 str56("Pyramid");
    ASSERT_EQ(str56.integer(), "Pyramid"_s56.integer());
    ASSERT_TRUE(str56.not_empty());

    strn::string56 small_str56("Cactus");
    ASSERT_EQ(small_str56.integer(), "Cactus"_s56.hash());
    ASSERT_TRUE(str56.not_empty());

    strn::string56 empty_str56("");
    ASSERT_EQ(empty_str56.integer(), ""_s56.hash());
    ASSERT_TRUE(empty_str56.empty());
}

TEST(string56_tests, test_constructor_c_str)
{
    const char* cstr = "Azerty";
    strn::string56 str56(cstr);
    ASSERT_EQ(str56.integer(), "Azerty"_s56.integer());
}

TEST(string56_tests, test_operator_eq_and_neq)
{
    strn::string56 str("orange");
    ASSERT_NE(str, ""_s56);
    ASSERT_NE("orange"_s56, ""_s56);
    ASSERT_EQ(str, "orange"_s56);
    strn::string56 str2("pomme");
    ASSERT_NE(str, str2);
}

TEST(string56_tests, test_hash)
{
    constexpr strn::string56::uint hash_init_value = 0x1122334455667707;
    enum hash_enum : strn::string56::uint
    {
        hash_value = hash_init_value
    };

    strn::string56 str2(hash_value);
    ASSERT_EQ(str2.integer(), hash_init_value);
    ASSERT_EQ(str2.hash(), static_cast<std::size_t>(hash_init_value));
}

TEST(string56_tests, test_hash_2)
{
    strn::string56 str("abcdefg");

    switch (str.hash())
    {
    case "abcdefg"_s56.hash():
        SUCCEED();
        break;
    default:
        FAIL() << "hash() method does not work.";
    }
}

TEST(string56_tests, test_to_string_view)
{
    strn::string56 str56("orange");
    std::string_view strv = str56.to_string_view();
    ASSERT_EQ(strv, "orange");
    str56 = "France"_s56;
    ASSERT_NE(strv, "orange");
    ASSERT_EQ(strv, "France");
}

TEST(string56_tests, test_to_string)
{
    strn::string56 str56("orange");
    std::string str = str56.to_string();
    ASSERT_EQ(str, "orange");
    str56 = "France"_s56;
    ASSERT_EQ(str, "orange");
}

TEST(string56_tests, test_empty_or_not)
{
    strn::string56 str56;
    ASSERT_TRUE(str56.empty());
    ASSERT_TRUE(!str56.not_empty());
    str56 = "full"_s56;
    ASSERT_TRUE(!str56.empty());
    ASSERT_TRUE(str56.not_empty());
}

TEST(string56_tests, test_length_empty)
{
    strn::string56 str56;
    ASSERT_EQ(str56.length(), 0);
}

TEST(string56_tests, test_length_small)
{
    strn::string56 str56 = "123"_s56;
    ASSERT_EQ(str56.length(), 3);
}

TEST(string56_tests, test_length_max)
{
    strn::string56 str56 = "1234567"_s56;
    ASSERT_EQ(str56.length(), 7);
    static_assert(strn::string56::max_length() == 7);
}

TEST(string56_tests, test_is_printable)
{
    strn::string56 str56("abcdefg");
    ASSERT_EQ(str56.length(), 7);
    ASSERT_TRUE(str56.is_printable());
    strn::string56 small_str56("abcd");
    ASSERT_TRUE(small_str56.is_printable());
    strn::string56 str56_not_printable("abc\1\2");
    ASSERT_FALSE(str56_not_printable.is_printable());
}

TEST(string56_tests, test_nth)
{
    strn::string56 str56("abc123d");
    ASSERT_EQ(str56[4], '2');
    str56[3] = 'k';
    ASSERT_EQ(str56[3], 'k');
    ASSERT_EQ(str56, "abck23d"_s56);
}

TEST(string56_tests, test_operator_less)
{
    strn::string56 str_aaa("aaa");
    strn::string56 str_bb("bb");
    ASSERT_LT(str_bb, str_aaa);
    ASSERT_LT(str_aaa.to_string_view(), str_bb.to_string_view());
}

TEST(string56_tests, test_push_back)
{
    strn::string56 str("aaa");
    char ch = 'b';
    str.push_back(ch);
    strn::string56 stra("aaa");
    strn::string56 strb("aaab");
    ASSERT_NE(str, stra);
    ASSERT_EQ(str, strb);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string56_tests, test_push_back_2)
{
    strn::string56 str("1234567");
    char ch = 'b';
    str.push_back(ch);
    strn::string56 expected_str("1234567");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, '7');
}

TEST(string56_tests, test_push_back_3)
{
    strn::string56 str;
    char ch = 'b';
    ASSERT_TRUE(str.empty());
    str.push_back(ch);
    ASSERT_TRUE(str.not_empty());

    strn::string56 expected_str("b");
    ASSERT_EQ(str, expected_str);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'b');
}

TEST(string56_tests, test_pop_back)
{
    strn::string56 str("aaab");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.not_empty());
    strn::string56 stra("aaa");
    strn::string56 strb("aaab");
    ASSERT_EQ(str, stra);
    ASSERT_NE(str, strb);
    const char& last_ch = *(str.end() - 1);
    ASSERT_EQ(last_ch, 'a');
}

TEST(string64_tests, test_pop_back_2)
{
    strn::string56 str("a");
    ASSERT_TRUE(str.not_empty());
    str.pop_back();
    ASSERT_TRUE(str.empty());
}

TEST(string56_tests, test_clear)
{
    strn::string56 str("aaab");
    str.clear();
    strn::string56 strb("aaab");
    ASSERT_TRUE(str.empty());
    ASSERT_NE(str, strb);
}

TEST(string56_tests, test_resize_shorter)
{
    strn::string56 str("aabbcc");
    str.resize(4);
    strn::string56 expected_str("aabb");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 4);
}

TEST(string56_tests, test_resize_same)
{
    strn::string56 str("aabbcc");
    str.resize(str.length());
    strn::string56 expected_str("aabbcc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 6);
}

TEST(string56_tests, test_resize_longer)
{
    strn::string56 str("aabb");
    str.resize(6, 'c');
    strn::string56 expected_str("aabbcc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), 6);
}

TEST(string56_tests, test_resize_too_much)
{
    strn::string56 str("aabb");
    str.resize(9, 'c');
    strn::string56 expected_str("aabbccc");
    ASSERT_EQ(str, expected_str);
    ASSERT_EQ(str.length(), str.max_length());
}

enum number : uint64_t
{
    ONE = "ONE"_s56.integer(),
    TWO = "TWO"_e56,
};

enum class color : uint64_t
{
    ORANGE = "ORANGE"_s56.integer(),
    BLUE = "BLUE"_e56,
};

enum class bad_enum56 : uint32_t
{
};

static_assert(strn::is_enum56_v<number>);
static_assert(strn::is_enum56_v<color>);
static_assert(!strn::is_enum56_v<bad_enum56>);

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
static_assert(constructible_with_enum_but_not_integer<strn::string56>());

TEST(string56_tests, test_enum_to_string_64)
{
    strn::string56 stra(ONE);
    ASSERT_EQ(stra, "ONE"_s56);
    strn::string56 strc = strn::string56(TWO);
    ASSERT_EQ(strc, "TWO"_s56);
    strn::string56 strb = strn::string56(color::ORANGE);
    ASSERT_EQ(strb, "ORANGE"_s56);
    strn::string56 strd = strn::string56(color::BLUE);
    ASSERT_EQ(strd, "BLUE"_s56);
}

TEST(string56_tests, test_enum_to_string)
{
    std::string stra = strn::string56(ONE).to_string();
    ASSERT_EQ(stra, "ONE");
    std::string strc = strn::string56(TWO).to_string();
    ASSERT_EQ(strc, "TWO");
    std::string strb = strn::string56(color::ORANGE).to_string();
    ASSERT_EQ(strb, "ORANGE");
    std::string strd = strn::string56(color::BLUE).to_string();
    ASSERT_EQ(strd, "BLUE");
}

TEST(string56_tests, test_string_64_to_enum)
{
    strn::string56 stra("ONE");
    ASSERT_EQ(stra.to_enum<number>(), ONE);
    strn::string56 strc("TWO");
    ASSERT_EQ(strc.to_enum<number>(), TWO);
    strn::string56 strb("ORANGE");
    ASSERT_EQ(strb.to_enum<color>(), color::ORANGE);
    strn::string56 strd("BLUE");
    ASSERT_EQ(strd.to_enum<color>(), color::BLUE);
}

TEST(string56_tests, test_std_hash)
{
    std::hash<strn::string56> std_hash;
    ASSERT_EQ(std_hash("final"_s56), "final"_s56.hash());
}

TEST(string56_tests, test_operator_read)
{
    std::istringstream stream("abcdefghi\t123");
    strn::string56 str;
    stream >> str;
    ASSERT_EQ(str, "abcdefg"_s56);
    ASSERT_EQ(str.integer(), "abcdefg"_s56.integer());

    std::istringstream small_stream("abc\t123");
    strn::string56 small_str;
    small_stream >> small_str;
    ASSERT_EQ(small_str, "abc"_s56);
    ASSERT_EQ(small_str.integer(), "abc"_s56.integer());
}

TEST(string56_tests, test_operator_write)
{
    strn::string56 str("1234567");
    std::ostringstream stream;
    stream << str;
    ASSERT_EQ(stream.str(), str.to_string_view());
}

TEST(string56_tests, test_operator_format)
{
    strn::string56 str_a("A123");
    ASSERT_EQ(std::format("{}", str_a), "A123");
    strn::string56 str_b("B123456");
    ASSERT_EQ(std::format("{}", str_b), "B123456");
}

TEST(string56_tests, test_constexpr)
{
    if constexpr ("a"_s56 == "a"_s56)
    {
        ASSERT_EQ("a"_s56, "a"_s56);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

TEST(string56_tests, test_constexpr_fail)
{
    if constexpr ("abcdefghi"_s56 != ""_s56)
    {
        ASSERT_EQ("abcdefghi"_s56, "#BADs56"_s56);
        return;
    }
    FAIL() << "constexpr test des not work.";
}

constexpr strn::string56 constexpr_str = "cstexpr";
