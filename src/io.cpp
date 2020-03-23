#include <strn/io.hpp>
#include <strn/string64.hpp>
#include <strn/string32.hpp>
#include <string>
#include <iostream>
#include <locale>

namespace strn
{
std::ostream& operator<<(std::ostream& stream, const string64& str)
{
    uint8_t i = 0;
    for (auto iter = str.begin(); stream && *iter && i < string64::max_length(); ++iter, ++i)
        stream.put(*iter);
    return stream;
}

std::istream& operator>>(std::istream& stream, string64& str)
{
    char ch = 0;
    std::locale loc = stream.getloc();
    auto iter = str.begin();
    for (uint8_t i = 0; stream && !stream.eof() && i < string64::max_length(); ++i, ++iter)
    {
        stream.get(ch);
        if (!std::isspace(ch, loc))
            *iter = ch;
        else
            break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const string32& str)
{
    uint8_t i = 0;
    for (auto iter = str.begin(); stream && *iter && i < string32::max_length(); ++iter, ++i)
        stream.put(*iter);
    return stream;
}

std::istream& operator>>(std::istream& stream, string32& str)
{
    char ch = 0;
    std::locale loc = stream.getloc();
    auto iter = str.begin();
    for (uint8_t i = 0; stream && !stream.eof() && i < string32::max_length(); ++i, ++iter)
    {
        stream.get(ch);
        if (!std::isspace(ch, loc))
            *iter = ch;
        else
            break;
    }
    return stream;
}

}
