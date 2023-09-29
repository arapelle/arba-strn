#pragma once

#include "c_str_traits.hpp"
#include "string_n_helper.hpp"
#include <cstring>
#include <string_view>
#include <string>
#include <array>
#include <functional>

inline namespace arba
{
namespace strn
{

template <class T>
inline constexpr bool is_enum64_v = std::is_enum_v<T> && (sizeof(T) == sizeof(uint64_t));

class string64;

inline namespace literals
{
inline constexpr string64 operator"" _s64(const char* cstr, std::size_t len);
inline constexpr uint64_t operator"" _e64(const char* cstr, std::size_t len);
}

/**
 * @brief The string64 class
 */
class string64
{
public:
    using uint = uint64_t;
    using value_type = std::string::value_type;

private:
    inline constexpr static std::size_t buffer_size_ = sizeof(uint);
    inline constexpr static std::size_t max_input_c_str_length_ = buffer_size_ + 1;
    using buffer_type_ = std::array<value_type, buffer_size_>;

public:
    using iterator = buffer_type_::iterator;
    using const_iterator = buffer_type_::const_iterator;

    /**
     * @brief string64
     */
    constexpr explicit string64()
        : integer_(0)
    {}

    template <class Enum>
        requires is_enum64_v<Enum>
    constexpr explicit string64(Enum value)
        : integer_(static_cast<string64::uint>(value))
    {}

    /**
     * @brief string64
     * @param str
     *
     * std::string_view strv;
     * strn::string64 s64(strv);
     */
    constexpr explicit string64(const std::string_view& str)
        : string64()
    {
        std::copy(str.begin(), str.begin() + std::min<std::size_t>(buffer_size_, str.length()), cstr_.begin());
    }

    /**
     * @brief string64
     * @param str
     *
     * std::string str;
     * strn::string64 s64(str);
     */
    constexpr explicit string64(const std::string& str)
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
    constexpr string64(const T& cstr)
        : cstr_{cstr[0], Tr::template nth<1>(cstr), Tr::template nth<2>(cstr), Tr::template nth<3>(cstr),
                Tr::template nth<4>(cstr), Tr::template nth<5>(cstr), Tr::template nth<6>(cstr),
                Tr::template nth<7>(cstr)}
    {}

    /**
     * Compile error:
     * strn::string64 s64("I23456789");
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

    constexpr const uint& integer() const { return integer_; }
    constexpr std::size_t hash () const { return static_cast<std::size_t>(integer_); }
    constexpr std::string_view to_string_view() const { return std::string_view(cstr_.data(), length()); }
    constexpr std::string to_string() const { return std::string(begin(), end()); }
    constexpr bool empty() const { return cstr_.front() == '\0'; }
    constexpr bool not_empty() const { return !empty(); }
    constexpr std::size_t length() const { return string_n_helper::string64_length_(integer_); }
    constexpr static std::size_t max_length () { return buffer_size_; }
    constexpr const_iterator begin() const { return cstr_.begin(); }
    constexpr iterator begin() { return cstr_.begin(); }
    constexpr const_iterator end() const { return begin() + length(); }
    constexpr iterator end()  { return begin() + length(); }
    constexpr const_iterator cbegin() const { return begin(); }
    constexpr const_iterator cend() const { return end(); }
    bool is_printable() const;
    constexpr const char& operator[](std::size_t index) const { return cstr_[index]; }
    constexpr char& operator[](std::size_t index) { return cstr_[index]; }
    constexpr bool operator==(const string64& rhs) const { return integer_ == rhs.integer_; }
    constexpr bool operator!=(const string64& rhs) const { return integer_ != rhs.integer_; }
    constexpr bool operator<(const string64& rhs) const { return integer_ < rhs.integer_; }
    void push_back(const char &ch);
    void pop_back();
    inline void clear() { integer_ = 0; }
    void resize(std::size_t new_length, char new_ch = char());

    template <class Enum>
        requires is_enum64_v<Enum>
    inline constexpr Enum to_enum() { return static_cast<Enum>(integer_); }

private:
    friend inline constexpr string64 literals::operator"" _s64(const char* cstr, std::size_t len);
    friend inline constexpr uint64_t literals::operator"" _e64(const char* cstr, std::size_t len);

    inline constexpr static string64 from_cstr_(const char* cstr, std::size_t len)
    {
        if (len <= max_length())
        {
            uint value = 0;
            static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);
            if constexpr (std::endian::native == std::endian::little)
            {
                for (uint8_t i = 0; i < len; ++i)
                {
                    uint vi = static_cast<std::size_t>(cstr[i]);
                    vi <<= (8*i);
                    value |= vi;
                }
            }
            else
            {
                for (uint8_t i = 0, last_i = len - 1; i < len; ++i)
                {
                    uint vi = static_cast<std::size_t>(cstr[i]);
                    vi <<= (8 * (last_i - i));
                    value |= vi;
                }
            }
            return string64(value);
        }
        return "#BAD_S64"_s64;
    }

    constexpr explicit string64(uint value)
        : integer_(value)
    {}

private:
    union
    {
        buffer_type_ cstr_;
        uint integer_;
    };
};
static_assert(sizeof(string64::uint) == sizeof(uint64_t));
static_assert(sizeof(string64) == sizeof(string64::uint));

// Literals

inline namespace literals
{
/// operator"" _s64
inline constexpr string64 operator"" _s64(const char* cstr, std::size_t len)
{
    return string64::from_cstr_(cstr, len);
}

/// operator"" _e64
inline constexpr string64::uint operator"" _e64(const char* cstr, std::size_t len)
{
    return string64::from_cstr_(cstr, len).integer();
}

}
}
}

namespace std
{
/**
 * @brief The hash<strn::string64> struct specialization
 */
template <>
struct hash<arba::strn::string64>
{
    inline std::size_t operator() (const arba::strn::string64& value) const noexcept { return value.hash(); }
};
}
