#pragma once

#include <strn/version.hpp>
#include "c_str_traits.hpp"
#include <cstring>
#include <string_view>
#include <string>
#include <array>
#include <functional>

namespace strn
{
/**
 * @brief The string56 class
 */
class string56
{
public:
    using uint = uint64_t;

private:
    inline constexpr static std::size_t buffer_size_ = sizeof(uint);
    inline constexpr static std::size_t max_input_c_str_length_ = buffer_size_;
    using buffer_type_ = std::array<char, buffer_size_>;

public:
    using iterator = buffer_type_::iterator;
    using const_iterator = buffer_type_::const_iterator;

    /**
     * @brief string56
     * @param value
     *
     * strn::string56 s56();
     * strn::string56 s56(1235);
     */
    inline constexpr explicit string56(uint value = 0)
        : integer_(value)
    {}

    /**
     * @brief string56
     * @param str
     *
     * std::string_view strv;
     * strn::string56 s56(strv);
     */
    explicit string56(const std::string_view& str);

    /**
     * @brief string56
     * @param str
     *
     * std::string str;
     * strn::string56 s56(str);
     */
    inline explicit string56(const std::string& str)
        : string56(std::string_view(str))
    {}

    /**
     * @brief string56
     * @param cstr
     *
     * strn::string56 s56("123A567");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<Tr::length <= max_input_c_str_length_ && is_c_str_n_v<T>, int> = 0>
    string56(const T& cstr)
      : cstr_{cstr[0], Tr::template nth<1>(cstr), Tr::template nth<2>(cstr), Tr::template nth<3>(cstr),
                       Tr::template nth<4>(cstr), Tr::template nth<5>(cstr), Tr::template nth<6>(cstr), Tr::length-1}
    {}

    /**
     * strn::string56 s56("123A5678");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<(!(Tr::length <= max_input_c_str_length_)) && is_c_str_n_v<T>, int> = 0>
    string56(const T& cstr) = delete;

    /**
     * @brief string56
     * @param cstr
     *
     * std::string str;
     * const char* cstr = str.c_str();
     * strn::string56 s56(cstr);
     */
    template <typename T, std::enable_if_t<is_c_str_v<T>, int> = 0>
    explicit string56(const T& cstr)
      : string56(std::string_view(cstr))
    {}

    inline constexpr const uint& integer() const { return integer_; }
    inline constexpr std::size_t hash () const { return static_cast<std::size_t>(integer_); }
    inline std::string_view to_string_view() const { return std::string_view(cstr_.data(), length()); }
    inline std::string to_string() const { return std::string(begin(), end()); }
    inline bool empty() const { return cstr_.front() == '\0'; }
    inline bool not_empty() const { return !empty(); }
    inline std::size_t length() const { return cstr_.back(); }
    inline constexpr static std::size_t max_length () { return buffer_size_-1; }
    inline const_iterator begin() const { return cstr_.begin(); }
    inline iterator begin() { return cstr_.begin(); }
    inline const_iterator end() const { return cstr_.begin() + length(); }
    inline iterator end() { return cstr_.begin() + length(); }
    inline const_iterator cbegin() const { return begin(); }
    inline const_iterator cend() const { return end(); }
    bool is_printable() const;
    inline const char& operator[](std::size_t index) const { return cstr_[index]; }
    inline char& operator[](std::size_t index) { return cstr_[index]; }
    inline constexpr bool operator==(const string56& rhs) const { return integer_ == rhs.integer_; }
    inline constexpr bool operator!=(const string56& rhs) const { return integer_ != rhs.integer_; }
    inline constexpr bool operator<(const string56& rhs) const { return integer_ < rhs.integer_; }
    void push_back(const char &ch);
    void pop_back();
    inline void clear() { integer_ = 0; }
    void resize(std::size_t new_length, char new_ch = char());

private:
    union
    {
        buffer_type_ cstr_;
        uint integer_ = 0;
    };
};
static_assert(sizeof(string56::uint) == sizeof(uint64_t));
static_assert(sizeof(string56) == sizeof(string56::uint));

inline namespace literals
{
/// operator"" _s56
inline constexpr string56 operator"" _s56(const char* cstr, std::size_t len)
{
    if (len <= string56::max_length())
    {
        string56::uint value = 0;
        string56::uint i = 0;
        for (; i < sizeof(std::size_t) && i < len; ++i)
        {
            string56::uint vi = static_cast<std::size_t>(cstr[i]);
            vi <<= (8*i);
            value |= vi;
        }
        value |= (i << 56);
        return string56(value);
    }
    return string56("#BADS56");
}
}

// To/from enum 56


template <class T>
inline constexpr bool is_enum56_v = std::is_enum_v<T> && (sizeof(T) == sizeof(string56));

template <class Enum, std::enable_if_t<is_enum56_v<Enum>, int> = 0>
inline constexpr string56 enum56_to_string56(const Enum& e)
{
    return string56(static_cast<string56::uint>(e));
}

template <class Enum, std::enable_if_t<is_enum56_v<Enum>, int> = 0>
inline std::string enum56_to_string(const Enum& e)
{
    return enum56_to_string56(e).to_string();
}

template <class Enum, std::enable_if_t<is_enum56_v<Enum>, int> = 0>
inline constexpr Enum enum56_to_enum(const string56& str)
{
    return static_cast<Enum>(str.integer());
}
}

namespace std
{
/**
 * @brief The hash<strn::string56> struct specialization
 */
template <>
struct hash<strn::string56>
{
    inline std::size_t operator() (const strn::string56& value) const { return value.hash(); }
};
}
