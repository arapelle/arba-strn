#pragma once

#include <strn/version.hpp>
#include "c_str_traits.hpp"
#include <cstring>
#include <string_view>
#include <string>
#include <array>
#include <functional>
#include <cstdint>

namespace strn
{
/**
 * @brief The string64 class
 */
class string64
{
public:
    using uint = uint64_t;

private:
    inline constexpr static std::size_t buffer_size_ = sizeof(uint);
    inline constexpr static std::size_t max_input_c_str_length_ = buffer_size_ + 1;
    using buffer_type_ = std::array<char, buffer_size_>;

public:
    using iterator = buffer_type_::iterator;
    using const_iterator = buffer_type_::const_iterator;

    /**
     * @brief string64
     * @param value
     *
     * strn::string64 s64();
     * strn::string64 s64(1235);
     */
    inline constexpr explicit string64(uint value = 0)
        : integer_(value)
    {}

    /**
     * @brief string64
     * @param str
     *
     * std::string_view strv;
     * strn::string64 s64(strv);
     */
    explicit string64(const std::string_view& str);

    /**
     * @brief string64
     * @param str
     *
     * std::string str;
     * strn::string64 s64(str);
     */
    inline explicit string64(const std::string& str)
        : string64(std::string_view(str))
    {}

    /**
     * @brief string64
     * @param cstr
     *
     * strn::string64 s64("123A5678");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<Tr::length <= max_input_c_str_length_ && is_c_str_n_v<T>, int> = 0>
    string64(const T& cstr)
      : cstr_{cstr[0], Tr::template nth<1>(cstr), Tr::template nth<2>(cstr), Tr::template nth<3>(cstr),
                       Tr::template nth<4>(cstr), Tr::template nth<5>(cstr), Tr::template nth<6>(cstr), Tr::template nth<7>(cstr)}
    {}

    /**
     * strn::string64 s64("123A56789");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<(!(Tr::length <= max_input_c_str_length_)) && is_c_str_n_v<T>, int> = 0>
    string64(const T& cstr) = delete;

    /**
     * @brief string64
     * @param cstr
     *
     * std::string str;
     * const char* cstr = str.c_str();
     * strn::string64 s64(cstr);
     */
    template <typename T, std::enable_if_t<is_c_str_v<T>, int> = 0>
    explicit string64(const T& cstr)
      : string64(std::string_view(cstr))
    {}

    inline constexpr const uint& integer() const { return integer_; }
    inline constexpr std::size_t hash () const { return static_cast<std::size_t>(integer_); }
    inline std::string_view to_string_view() const { return std::string_view(cstr_.data(), length()); }
    inline std::string to_string() const { return std::string(begin(), end()); }
    inline bool empty() const { return cstr_.front() == '\0'; }
    inline bool not_empty() const { return !empty(); }
    inline std::size_t length() const { return cstr_.back() ? max_length() : std::strlen(cstr_.data()); }
    inline constexpr static std::size_t max_length () { return buffer_size_; }
    inline const_iterator begin() const { return cstr_.begin(); }
    inline iterator begin() { return cstr_.begin(); }
    const_iterator end() const;
    iterator end();
    inline const_iterator cbegin() const { return begin(); }
    inline const_iterator cend() const { return end(); }
    bool is_printable() const;
    inline const char& operator[](std::size_t index) const { return cstr_[index]; }
    inline char& operator[](std::size_t index) { return cstr_[index]; }
    inline constexpr bool operator==(const string64& rhs) const { return integer_ == rhs.integer_; }
    inline constexpr bool operator!=(const string64& rhs) const { return integer_ != rhs.integer_; }
    inline constexpr bool operator<(const string64& rhs) const { return integer_ < rhs.integer_; }
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
static_assert(sizeof(string64::uint) == sizeof(uint64_t));
static_assert(sizeof(string64) == sizeof(string64::uint));

inline namespace literals
{
/// operator"" _s64
inline constexpr string64 operator"" _s64(const char* cstr, std::size_t len)
{
    if (len <= string64::max_length())
    {
        string64::uint value = 0;
        for (uint8_t i = 0; i < sizeof(std::size_t) && i < len; ++i)
        {
            string64::uint vi = static_cast<std::size_t>(cstr[i]);
            vi <<= (8*i);
            value |= vi;
        }
        return string64(value);
    }
    return string64("#BAD_S64");
}
}

// To/from enum 64


template <class T>
inline constexpr bool is_enum64_v = std::is_enum_v<T> && (sizeof(T) == sizeof(string64));

template <class Enum, std::enable_if_t<is_enum64_v<Enum>, int> = 0>
inline constexpr string64 enum64_to_string64(const Enum& e)
{
    return string64(static_cast<string64::uint>(e));
}

template <class Enum, std::enable_if_t<is_enum64_v<Enum>, int> = 0>
inline std::string enum64_to_string(const Enum& e)
{
    return enum64_to_string64(e).to_string();
}

template <class Enum, std::enable_if_t<is_enum64_v<Enum>, int> = 0>
inline constexpr Enum string64_to_enum(const string64& str)
{
    return static_cast<Enum>(str.integer());
}
}

namespace std
{
/**
 * @brief The hash<strn::string64> struct specialization
 */
template <>
struct hash<strn::string64>
{
    inline std::size_t operator() (const strn::string64& value) const { return value.hash(); }
};
}
