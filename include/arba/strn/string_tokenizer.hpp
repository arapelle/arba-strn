#pragma once

#include <algorithm>
#include <ranges>
#include <string_view>

inline namespace arba
{
namespace strn
{

template <class ch_type>
struct char_tokenizer
{
    explicit char_tokenizer(ch_type sep_ch = 0) : sep_ch_(sep_ch) {}

    inline bool operator()(const ch_type& ch) const { return ch == sep_ch_; }
    inline bool operator==(const char_tokenizer&) const = default;
    inline bool operator!=(const char_tokenizer&) const = default;

private:
    ch_type sep_ch_;
};

template <class ch_type>
struct char_set_tokenizer
{
    explicit char_set_tokenizer(std::basic_string_view<ch_type> sep_chs = 0) : sep_chs_(sep_chs) {}

    inline bool operator()(const ch_type& ch) const { return sep_chs_.find(ch) != sep_chs_.npos; }
    inline bool operator==(const char_set_tokenizer&) const = default;
    inline bool operator!=(const char_set_tokenizer&) const = default;

private:
    std::basic_string<ch_type> sep_chs_;
};

template <class tokenizer_func, class ch_type>
class string_view_token_iterator
{
public:
    using char_type = ch_type;
    using string_view = std::basic_string_view<char_type>;
    using tokenizer_function = tokenizer_func;
    // input_iterator requirements:
    using value_type = string_view;
    using difference_type = std::ptrdiff_t;

private:
    using string_view_iterator = typename string_view::iterator;

public:
    inline string_view_token_iterator()
        : is_ch_sep_(nullptr), str_end_iter_(std::string_view().end()), token_begin_iter_(std::string_view().end()),
          token_end_iter_(std::string_view().end())
    {
    }

    explicit string_view_token_iterator(string_view str, tokenizer_function& is_ch_sep)
        : is_ch_sep_(&is_ch_sep), str_end_iter_(str.end()),
          token_begin_iter_(std::find_if_not(str.begin(), str_end_iter_, *is_ch_sep_)),
          token_end_iter_(std::find_if(token_begin_iter_, str_end_iter_, *is_ch_sep_))
    {
    }

    string_view operator*() const
    {
        return std::string_view(&*token_begin_iter_, std::distance(token_begin_iter_, token_end_iter_));
    }

    string_view_token_iterator& operator++()
    {
        if (token_end_iter_ != str_end_iter_)
        {
            token_begin_iter_ = std::find_if_not(token_end_iter_ + 1, str_end_iter_, *is_ch_sep_);
            token_end_iter_ = std::find_if(token_begin_iter_, str_end_iter_, *is_ch_sep_);
        }
        else
        {
            *this = string_view_token_iterator();
        }
        return *this;
    }

    string_view_token_iterator operator++(int)
    {
        string_view_token_iterator iter(*this);
        ++(*this);
        return iter;
    }

    inline bool operator==(const string_view_token_iterator&) const = default;
    inline bool operator!=(const string_view_token_iterator&) const = default;

private:
    tokenizer_function* is_ch_sep_ = nullptr;
    string_view_iterator str_end_iter_;
    string_view_iterator token_begin_iter_;
    string_view_iterator token_end_iter_;
};

template <class tokenizer_func, class ch_type>
class string_token_iterator : public string_view_token_iterator<tokenizer_func, ch_type>
{
private:
    using base_ = string_view_token_iterator<tokenizer_func, ch_type>;

public:
    using typename base_::char_type;
    using typename base_::string_view;
    using typename base_::tokenizer_function;
    using string = std::basic_string<char_type>;
    using value_type = string;

public:
    inline string_token_iterator() {}

    explicit string_token_iterator(string_view str, tokenizer_function& is_ch_sep) : base_(str, is_ch_sep) {}

    string operator*() const { return std::string(*(static_cast<const base_&>(*this))); }

    string_token_iterator& operator++()
    {
        ++static_cast<base_&>(*this);
        return *this;
    }

    string_token_iterator operator++(int)
    {
        string_token_iterator iter(*this);
        ++(*this);
        return iter;
    }
};

namespace impl
{

template <class tokenizer_function>
class str_tokenizer_function_owner_
{
public:
    str_tokenizer_function_owner_() = default;
    str_tokenizer_function_owner_(tokenizer_function is_ch_sep) : is_ch_sep_(std::move(is_ch_sep)) {}

protected:
    tokenizer_function is_ch_sep_;
};

template <class tokenizer_function, class token_iterator>
class str_tokenizer_ : public str_tokenizer_function_owner_<tokenizer_function>,
                       public std::ranges::subrange<token_iterator>
{
public:
    using char_type = typename token_iterator::char_type;
    using string_view = std::basic_string_view<char_type>;

public:
    str_tokenizer_() = default;
    str_tokenizer_(string_view str, tokenizer_function is_ch_sep)
        : str_tokenizer_function_owner_<tokenizer_function>(std::move(is_ch_sep)),
          std::ranges::subrange<token_iterator>(token_iterator(str, this->is_ch_sep_), token_iterator())
    {
    }
};

} // namespace impl

// string_view_tokenizer:

template <class tokenizer_func, class char_type = char>
class string_view_tokenizer;

template <class tokenizer_func, class char_type>
class string_view_tokenizer
    : public impl::str_tokenizer_<tokenizer_func, string_view_token_iterator<tokenizer_func, char_type>>
{
private:
    using base_ = impl::str_tokenizer_<tokenizer_func, string_view_token_iterator<tokenizer_func, char_type>>;

public:
    string_view_tokenizer() = default;
    string_view_tokenizer(typename base_::string_view str, tokenizer_func is_ch_sep) : base_(str, std::move(is_ch_sep))
    {
    }
};

template <class char_type>
class string_view_tokenizer<char_type, char_type>
    : public impl::str_tokenizer_<char_tokenizer<char>,
                                  string_view_token_iterator<char_tokenizer<char_type>, char_type>>
{
private:
    using base_ =
        impl::str_tokenizer_<char_tokenizer<char>, string_view_token_iterator<char_tokenizer<char_type>, char_type>>;

public:
    string_view_tokenizer() = default;
    string_view_tokenizer(typename base_::string_view str, char_type ch_sep) : base_(str, char_tokenizer<char>(ch_sep))
    {
    }
};

template <class char_type>
class string_view_tokenizer<const char_type*, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ = impl::str_tokenizer_<char_set_tokenizer<char>,
                                       string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_view_tokenizer() = default;
    string_view_tokenizer(typename base_::string_view str, const char_type* ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

template <class char_type>
class string_view_tokenizer<std::basic_string_view<char_type>, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ = impl::str_tokenizer_<char_set_tokenizer<char>,
                                       string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_view_tokenizer() = default;
    string_view_tokenizer(typename base_::string_view str, std::basic_string_view<char_type> ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

template <class char_type>
class string_view_tokenizer<std::basic_string<char_type>, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ = impl::str_tokenizer_<char_set_tokenizer<char>,
                                       string_view_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_view_tokenizer() = default;
    string_view_tokenizer(typename base_::string_view str, std::basic_string_view<char_type> ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

// string_tokenizer:

template <class tokenizer_func, class char_type = char>
class string_tokenizer;

template <class tokenizer_func, class char_type>
class string_tokenizer : public impl::str_tokenizer_<tokenizer_func, string_token_iterator<tokenizer_func, char_type>>
{
private:
    using base_ = impl::str_tokenizer_<tokenizer_func, string_token_iterator<tokenizer_func, char_type>>;

public:
    string_tokenizer() = default;
    string_tokenizer(typename base_::string_view str, tokenizer_func is_ch_sep) : base_(str, std::move(is_ch_sep)) {}
};

template <class char_type>
class string_tokenizer<char_type, char_type>
    : public impl::str_tokenizer_<char_tokenizer<char>, string_token_iterator<char_tokenizer<char_type>, char_type>>
{
private:
    using base_ =
        impl::str_tokenizer_<char_tokenizer<char>, string_token_iterator<char_tokenizer<char_type>, char_type>>;

public:
    string_tokenizer() = default;
    string_tokenizer(typename base_::string_view str, char_type ch_sep) : base_(str, char_tokenizer<char>(ch_sep)) {}
};

template <class char_type>
class string_tokenizer<const char_type*, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ =
        impl::str_tokenizer_<char_set_tokenizer<char>, string_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_tokenizer() = default;
    string_tokenizer(typename base_::string_view str, const char_type* ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

template <class char_type>
class string_tokenizer<std::basic_string_view<char_type>, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ =
        impl::str_tokenizer_<char_set_tokenizer<char>, string_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_tokenizer() = default;
    string_tokenizer(typename base_::string_view str, std::basic_string_view<char_type> ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

template <class char_type>
class string_tokenizer<std::basic_string<char_type>, char_type>
    : public impl::str_tokenizer_<char_set_tokenizer<char>,
                                  string_token_iterator<char_set_tokenizer<char_type>, char_type>>
{
private:
    using base_ =
        impl::str_tokenizer_<char_set_tokenizer<char>, string_token_iterator<char_set_tokenizer<char_type>, char_type>>;

public:
    string_tokenizer() = default;
    string_tokenizer(typename base_::string_view str, std::basic_string_view<char_type> ch_seps)
        : base_(str, char_set_tokenizer<char>(ch_seps))
    {
    }
};

} // namespace strn
} // namespace arba
