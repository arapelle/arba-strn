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
 * @brief The string32 class
 */
class string32
{
public:
    using uint = uint32_t;

private:
    inline constexpr static std::size_t buffer_size_ = sizeof(uint);
    inline constexpr static std::size_t max_input_c_str_length_ = buffer_size_ + 1;
    using buffer_type_ = std::array<char, buffer_size_>;

public:
    using iterator = buffer_type_::iterator;
    using const_iterator = buffer_type_::const_iterator;

    /**
     * @brief string32
     * @param value
     *
     * strn::string32 s32();
     * strn::string32 s32(1235);
     */
    inline constexpr explicit string32(uint value = 0)
        : integer_(value)
    {}

    /**
     * @brief string32
     * @param str
     *
     * std::string_view strv;
     * strn::string32 s32(strv);
     */
    explicit string32(const std::string_view& str);

    /**
     * @brief string32
     * @param str
     *
     * std::string str;
     * strn::string32 s32(str);
     */
    inline explicit string32(const std::string& str)
        : string32(std::string_view(str))
    {}

    /**
     * @brief string32
     * @param cstr
     *
     * strn::string32 s32("12E4");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<Tr::length <= max_input_c_str_length_ && is_c_str_n_v<T>, int> = 0>
    string32(const T& cstr)
      : cstr_{cstr[0], Tr::template nth<1>(cstr), Tr::template nth<2>(cstr), Tr::template nth<3>(cstr)}
    {}

    /**
     * strn::string32 s32("12E45");
     */
    template <typename T, typename Tr = c_str_n_traits<T>,
            std::enable_if_t<(!(Tr::length <= max_input_c_str_length_)) && is_c_str_n_v<T>, int> = 0>
    string32(const T& cstr) = delete;

    /**
     * @brief string32
     * @param cstr
     *
     * std::string str;
     * const char* cstr = str.c_str();
     * strn::string32 s32(cstr);
     */
    template <typename T, std::enable_if_t<is_c_str_v<T>, int> = 0>
    explicit string32(const T& cstr)
      : string32(std::string_view(cstr))
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
    inline constexpr bool operator==(const string32& rhs) const { return integer_ == rhs.integer_; }
    inline constexpr bool operator!=(const string32& rhs) const { return integer_ != rhs.integer_; }
    inline constexpr bool operator<(const string32& rhs) const { return integer_ < rhs.integer_; }
    void push_back(const char &ch);

private:
    union
    {
        buffer_type_ cstr_;
        uint integer_ = 0;
    };
};
static_assert(sizeof(string32::uint) == sizeof(uint32_t));
static_assert(sizeof(string32) == sizeof(string32::uint));

inline namespace literals
{
/// operator"" _s32
inline constexpr string32 operator"" _s32(const char* cstr, std::size_t len)
{
    if (len <= string32::max_length())
    {
        string32::uint value = 0;
        for (uint8_t i = 0; i < sizeof(std::size_t) && i < len; ++i)
        {
            string32::uint vi = static_cast<std::size_t>(cstr[i]);
            vi <<= (8*i);
            value |= vi;
        }
        return string32(value);
    }
    return string32("#BAD");
}
}

// To/from enum 32


template <class T>
inline constexpr bool is_enum32_v = std::is_enum_v<T> && (sizeof(T) == sizeof(string32));

template <class Enum, std::enable_if_t<is_enum32_v<Enum>, int> = 0>
inline constexpr string32 enum32_to_string32(const Enum& e)
{
    return string32(static_cast<string32::uint>(e));
}

template <class Enum, std::enable_if_t<is_enum32_v<Enum>, int> = 0>
inline std::string enum32_to_string(const Enum& e)
{
    return enum32_to_string32(e).to_string();
}

template <class Enum, std::enable_if_t<is_enum32_v<Enum>, int> = 0>
inline constexpr Enum enum32_to_enum(const string32& str)
{
    return static_cast<Enum>(str.integer());
}
}

namespace std
{
/**
 * @brief The hash<strn::string32> struct specialization
 */
template <>
struct hash<strn::string32>
{
    inline std::size_t operator() (const strn::string32& value) const { return value.hash(); }
};
}
