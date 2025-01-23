#include <arba/strn/string_tokenizer.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <iterator>
#include <ranges>

// string_view tests:

TEST(string_view_tokenizer_tests, test_pre_inc)
{
    using tokenizer_function = strn::char_tokenizer<char>;
    using string_view_token_iterator = strn::string_view_token_iterator<tokenizer_function, char>;

    std::string_view str("Some beautiful text");
    tokenizer_function tk_func(' ');
    string_view_token_iterator tokens_iter(str, tk_func);
    ASSERT_NE(tokens_iter, string_view_token_iterator());
    ASSERT_EQ(*tokens_iter, "Some");
    auto first_iter = tokens_iter;
    auto inc_iter = ++tokens_iter;
    ASSERT_NE(inc_iter, first_iter);
    ASSERT_EQ(inc_iter, tokens_iter);
    ASSERT_EQ(*tokens_iter, "beautiful");
    ++tokens_iter;
    ASSERT_EQ(*tokens_iter, "text");
    ++tokens_iter;
    ASSERT_EQ(tokens_iter, string_view_token_iterator());
}

TEST(string_view_tokenizer_tests, test_post_inc)
{
    using tokenizer_function = strn::char_tokenizer<char>;
    using string_view_token_iterator = strn::string_view_token_iterator<tokenizer_function, char>;

    std::string_view str("Some beautiful text");
    tokenizer_function tk_func(' ');
    string_view_token_iterator tokens_iter(str, tk_func);
    ASSERT_NE(tokens_iter, string_view_token_iterator());
    auto first_iter = tokens_iter;
    ASSERT_EQ(*tokens_iter, "Some");
    auto inc_iter = tokens_iter++;
    ASSERT_NE(inc_iter, tokens_iter);
    ASSERT_EQ(inc_iter, first_iter);
    ASSERT_EQ(*tokens_iter, "beautiful");
    tokens_iter++;
    ASSERT_EQ(*tokens_iter, "text");
    tokens_iter++;
    ASSERT_EQ(tokens_iter, string_view_token_iterator());
}

TEST(string_view_tokenizer_tests, test_tokenizer_char)
{
    std::string_view str("Some beautiful text");
    strn::string_view_tokenizer str_tokens(str, ' ');

    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_view_tokenizer_tests, test_tokenizer_char_set_cstr)
{
    std::string_view str("Some beautiful,text");
    strn::string_view_tokenizer str_tokens(str, " ,");

    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_view_tokenizer_tests, test_tokenizer_char_set_strv)
{
    std::string_view str("Some beautiful,text");
    strn::string_view_tokenizer str_tokens(str, std::string_view(" ,"));

    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_view_tokenizer_tests, test_tokenizer_char_set_str)
{
    std::string_view str("Some beautiful,text");
    std::string seps(" ,");
    strn::string_view_tokenizer str_tokens(str, seps);

    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_view_tokenizer_tests, test_tokenizer_function)
{
    std::string_view str("Some   beautiful\ttext");
    strn::string_view_tokenizer str_tokens(str, isspace);

    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_view_tokenizer_tests, test_tokenizer_functor)
{
    std::string_view str("Some   beautiful\ttext.cool");
    strn::string_view_tokenizer str_tokens(str, [](char ch) { return isspace(ch) || ispunct(ch); });
    std::vector<std::string_view> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string_view> expected_sv_tokens{ "Some", "beautiful", "text", "cool" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

// string tests:

TEST(string_tokenizer_tests, test_pre_inc)
{
    using tokenizer_function = strn::char_tokenizer<char>;
    using string_token_iterator = strn::string_token_iterator<tokenizer_function, char>;

    std::string_view str("Some beautiful text");
    tokenizer_function tk_func(' ');
    string_token_iterator tokens_iter(str, tk_func);
    ASSERT_NE(tokens_iter, string_token_iterator());
    ASSERT_EQ(*tokens_iter, "Some");
    auto first_iter = tokens_iter;
    auto inc_iter = ++tokens_iter;
    ASSERT_NE(inc_iter, first_iter);
    ASSERT_EQ(inc_iter, tokens_iter);
    ASSERT_EQ(*tokens_iter, "beautiful");
    ++tokens_iter;
    ASSERT_EQ(*tokens_iter, "text");
    ++tokens_iter;
    ASSERT_EQ(tokens_iter, string_token_iterator());
}

TEST(string_tokenizer_tests, test_post_inc)
{
    using tokenizer_function = strn::char_tokenizer<char>;
    using string_token_iterator = strn::string_token_iterator<tokenizer_function, char>;

    std::string_view str("Some beautiful text");
    tokenizer_function tk_func(' ');
    string_token_iterator tokens_iter(str, tk_func);
    ASSERT_NE(tokens_iter, string_token_iterator());
    auto first_iter = tokens_iter;
    ASSERT_EQ(*tokens_iter, "Some");
    auto inc_iter = tokens_iter++;
    ASSERT_NE(inc_iter, tokens_iter);
    ASSERT_EQ(inc_iter, first_iter);
    ASSERT_EQ(*tokens_iter, "beautiful");
    tokens_iter++;
    ASSERT_EQ(*tokens_iter, "text");
    tokens_iter++;
    ASSERT_EQ(tokens_iter, string_token_iterator());
}

TEST(string_tokenizer_tests, test_tokenizer_char)
{
    std::string_view str("Some beautiful text");
    strn::string_tokenizer str_tokens(str, ' ');

    std::vector<std::string> s_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(s_tokens));
    std::vector<std::string> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(s_tokens, expected_sv_tokens);

    //    std::ranges::subrange<strn::string_view_token_iterator<char>>(strn::string_view_token_iterator<char>(str, '
    //    '), strn::string_view_token_iterator<char>());
}

TEST(string_tokenizer_tests, test_tokenizer_char_set_cstr)
{
    std::string_view str("Some beautiful,text");
    strn::string_tokenizer str_tokens(str, " ,");

    std::vector<std::string> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_tokenizer_tests, test_tokenizer_char_set_strv)
{
    std::string_view str("Some beautiful,text");
    strn::string_tokenizer str_tokens(str, std::string_view(" ,"));

    std::vector<std::string> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_tokenizer_tests, test_tokenizer_char_set_str)
{
    std::string_view str("Some beautiful,text");
    std::string seps(" ,");
    strn::string_tokenizer str_tokens(str, seps);

    std::vector<std::string> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_tokenizer_tests, test_tokenizer_function)
{
    std::string_view str("Some   beautiful\ttext");
    strn::string_tokenizer str_tokens(str, isspace);

    std::vector<std::string> sv_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(sv_tokens));
    std::vector<std::string> expected_sv_tokens{ "Some", "beautiful", "text" };
    ASSERT_EQ(sv_tokens, expected_sv_tokens);
}

TEST(string_tokenizer_tests, test_tokenizer_functor)
{
    std::string_view str("Some   beautiful\ttext.cool");
    strn::string_tokenizer str_tokens(str, [](char ch) { return isspace(ch) || ispunct(ch); });
    std::vector<std::string> v_tokens;
    std::ranges::copy(str_tokens, std::back_inserter(v_tokens));
    std::vector<std::string> expected_v_tokens{ "Some", "beautiful", "text", "cool" };
    ASSERT_EQ(v_tokens, expected_v_tokens);
}
