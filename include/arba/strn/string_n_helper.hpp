#pragma once

#include <cstdint>
#include <bit>

inline namespace arba
{
namespace strn
{

class string_n_helper
{
    friend class string16;
    friend class string32;
    friend class string64;

    inline constexpr static std::size_t string16_length_(const uint16_t& bytes)
    {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);
        static_assert(static_cast<std::size_t>(static_cast<bool>(true)) == std::size_t{1});

        if constexpr (std::endian::native == std::endian::little)
        {
            if ((bytes & 0xff00))
                return 2;
        }
        else
        {
            if ((bytes & 0x00ff))
                return 2;
        }
        return static_cast<std::size_t>(static_cast<bool>(bytes));
    }

    inline constexpr static std::size_t string32_length_(const uint32_t& bytes)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            if ((bytes & 0xffff0000))
                return 2 + string16_length_(bytes >> 16);
        }
        else
        {
            if ((bytes & 0x0000ffff))
                return 2 + string16_length_(bytes << 16);
        }
        return string16_length_(bytes);
    }

    inline constexpr static std::size_t string64_length_(const uint64_t& bytes)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            if ((bytes & 0xffffffff00000000))
                return 4 + string32_length_(bytes >> 32);
        }
        else
        {
            if ((bytes & 0x00000000ffffffff))
                return 4 + string32_length_(bytes << 32);
        }
        return string32_length_(bytes);
    }
};

}
}
