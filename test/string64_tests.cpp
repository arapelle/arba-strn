#include <strn/io.hpp>
#include <strn/string64.hpp>
#include <sstream>
#include <cstdlib>
#include <cassert>

using namespace strn::literals;

void test_hash_literal()
{
    std::size_t hash = "c12"_hash;
    assert(hash == ('c'|('1'<<8)|('2'<<16)));
}

void test_constructor_empty()
{
    strn::string64 str64;
    assert(str64.integer() == 0);
    assert(str64.hash() == 0);
}

void test_constructor_std_string_view()
{
    std::string str("c12");
    std::string strv(str);
    strn::string64 str64(strv);
    assert(str64.integer() == "c12"_hash);
    assert(str64.hash() == "c12"_hash);
}

void test_constructor_too_long_std_string_view()
{
    std::string str("123456789");
    std::string strv(str);
    strn::string64 str64(strv);
    assert(str64.integer() == "12345678"_hash);
    assert(str64.hash() == "12345678"_hash);
}

void test_str_correctness ()
{
    strn::string64 str64("orange");
    assert(str64.to_string() == "orange");
    assert(str64.integer() != 0);
    assert(str64.hash() != 0);
    assert(str64.integer() == "orange"_hash);
    assert(str64.hash() == "orange"_hash);
}

void test_constructor_std_string()
{
    std::string str("46578c");
    strn::string64 str64(str);
    assert(str64.integer() == "46578c"_hash);
    assert(str64.hash() == "46578c"_hash);
}

void test_constructor_c_str_n()
{
    strn::string64 str64("Cactus");
    assert(str64.integer() == "Cactus"_hash);
}

void test_constructor_c_str()
{
    const char* cstr = "Azerty";
    strn::string64 str64(cstr);
    assert(str64.integer() == "Azerty"_hash);
}

void test_operator_eq_and_neq()
{
    strn::string64 str("orange");
    assert(str != ""_s64);
    assert("orange"_s64 != ""_s64);
    assert(str == "orange"_s64);
    strn::string64 str2("pomme");
    assert(str != str2);
}

void test_hash()
{
    strn::string64 str("abcdefgh");

    switch (str.hash())
    {
    case "abcdefgh"_hash:
        assert(true);
        break;
    default:
        assert(false);
    }
}

void test_to_string_view()
{
    strn::string64 str64("orange");
    std::string_view strv = str64.to_string_view();
    assert(strv == "orange");
    str64 = "France"_s64;
    assert(strv != "orange");
    assert(strv == "France");
}

void test_to_string()
{
    strn::string64 str64("orange");
    std::string str = str64.to_string();
    assert(str == "orange");
    str64 = "France"_s64;
    assert(str == "orange");
}

void test_empty_or_not()
{
    strn::string64 str64;
    assert(str64.empty());
    assert(!str64.not_empty());
    str64 = "full"_s64;
    assert(!str64.empty());
    assert(str64.not_empty());
}

void test_length()
{
    strn::string64 str64;
    assert(str64.length() == 0);
    str64 = "123"_s64;
    assert(str64.length() == 3);
    str64 = "12345678"_s64;
    assert(str64.length() == 8);
    static_assert(strn::string64::max_length() == 8);
}

void test_iterators()
{
    strn::string64 str64("abcdefgh");
    assert(*str64.begin() == 'a');
    assert(std::distance(str64.begin(), str64.end()) == 8);
}

void test_nth()
{
    strn::string64 str64("abc123d");
    assert(str64[4] == '2');
    str64[3] = 'k';
    assert(str64[3] == 'k');
    assert(str64 == "abck23d"_s64);
}

void test_operator_less()
{
    strn::string64 str_aaa("aaa");
    strn::string64 str_bb("bb");
    assert(str_bb < str_aaa);
    assert(str_aaa.to_string_view() < str_bb.to_string_view());
}

enum number : uint64_t
{
    ONE = "ONE"_hash,
    TWO = "TWO"_hash,
};

enum class color : uint64_t
{
    ORANGE = "ORANGE"_hash,
    BLUE = "BLUE"_hash,
};

enum class bad_enum64 : uint32_t
{
};

static_assert(strn::is_enum64_v<number>);
static_assert(strn::is_enum64_v<color>);
static_assert(!strn::is_enum64_v<bad_enum64>);

void test_enum_to_string_64()
{
    strn::string64 stra = strn::to_string64(ONE);
    assert(stra == "ONE"_s64);
    strn::string64 strb = strn::to_string64(color::ORANGE);
    assert(strb == "ORANGE"_s64);
}

void test_enum_to_string()
{
    std::string stra = strn::to_string(ONE);
    assert(stra == "ONE");
    std::string strb = strn::to_string(color::ORANGE);
    assert(strb == "ORANGE");
}

void test_string_64_to_enum()
{
    strn::string64 stra("ONE");
    assert(strn::to_enum<number>(stra) == ONE);
    strn::string64 strb("ORANGE");
    assert(strn::to_enum<color>(strb) == color::ORANGE);
}

void test_std_hash()
{
    std::hash<strn::string64> std_hash;
    assert(std_hash("final"_s64) == "final"_hash);
}

void test_operator_read()
{
    std::istringstream stream("abc\t123");
    strn::string64 str;
    stream >> str;
    assert(str == "abc"_s64);
    assert(str.hash() == "abc"_hash);
}

void test_operator_write()
{
    strn::string64 str("12345678");
    std::ostringstream stream;
    stream << str;
    assert(stream.str() == str.to_string_view());
}

void test_constexpr()
{
    if constexpr ("a"_s64 == "a"_s64)
    {
        assert("a"_s64 == "a"_s64);
        return;
    }
    assert(false);
}

int main ()
{
    test_hash_literal();
    test_constructor_empty();
    test_constructor_too_long_std_string_view();
    test_constructor_std_string_view();
    test_str_correctness();
    test_constructor_std_string();
    test_constructor_c_str_n();
    test_constructor_c_str();
    test_operator_eq_and_neq();
    test_hash();
    test_to_string_view();
    test_to_string();
    test_empty_or_not();
    test_length();
    test_iterators();
    test_nth();
    test_operator_less();
    test_enum_to_string_64();
    test_enum_to_string();
    test_string_64_to_enum();
    test_std_hash();
    test_operator_read();
    test_operator_write();
    test_constexpr();

    return EXIT_SUCCESS;
}
