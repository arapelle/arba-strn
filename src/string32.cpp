#include <strn/string32.hpp>

namespace strn
{
string32::string32(const std::string_view& str)
    : string32()
{
    std::copy(str.begin(), str.begin() + std::min<std::size_t>(buffer_size_, str.length()), cstr_.begin());
}

string32::const_iterator string32::end() const
{
    const_iterator iter = begin();
    for (uint8_t i = 0; *iter && i < buffer_size_; ++iter, ++i);
    return iter;
}

string32::iterator string32::end()
{
    iterator iter = begin();
    for (uint8_t i = 0; *iter && i < buffer_size_; ++iter, ++i);
    return iter;
}

bool string32::is_printable() const
{
    for (buffer_type_::const_iterator iter = cstr_.begin(), end_iter = cstr_.end();
         iter != end_iter;
         ++iter)
    {
        if (*iter == 0)
            return true;
        if (!isprint(*iter))
            return false;
    }
    return true;
}

void string32::push_back(const char& ch)
{
    iterator end_iter = end();
    if (std::size_t length = end_iter - begin(); length < max_length())
        *end_iter = ch;
}

}
