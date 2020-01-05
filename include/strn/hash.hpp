#pragma once

#include <string_view>
#include <functional>
#include <cstdint>
#include <cstdlib>

namespace strn
{
inline namespace literals
{
constexpr std::size_t operator"" _hash(const char* cstr, std::size_t len)
{
    if (len <= sizeof(std::size_t))
    {
        std::size_t hash = 0;
        for (uint8_t i = 0; i < sizeof(std::size_t) && i < len; ++i)
        {
            std::size_t vi = static_cast<std::size_t>(cstr[i]);
            vi <<= (8*i);
            hash |= vi;
        }
        return hash;
    }
    return std::hash<std::string_view>{}(cstr);
}

}
}
