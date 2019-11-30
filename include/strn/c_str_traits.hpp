#pragma once

#include <type_traits>

namespace strn
{
template <typename T>
struct c_str_n_traits
{
};

template <std::size_t N>
struct c_str_n_traits<char[N]>
{
    inline static constexpr std::size_t length = N;

    template <std::size_t I, std::enable_if_t<!(I>=N), int> = 0>
    inline static char nth(const char(& cstr)[N]) { return cstr[I]; }

    template <std::size_t I, std::enable_if_t<(I>=N), int> = 0>
    inline static char nth(const char(&)[N]) { return '\0'; }
};

template <typename T>
struct is_c_str_n : public std::false_type
{
};

template <std::size_t N>
struct is_c_str_n<char[N]> : public std::true_type
{
};

template <class T>
inline constexpr bool is_c_str_n_v = is_c_str_n<T>::value;

template <typename T>
struct is_c_str : public std::true_type
{
};

template <std::size_t N>
struct is_c_str<char[N]> : public std::false_type
{
};

template <class T>
inline constexpr bool is_c_str_v = is_c_str<T>::value;
}
