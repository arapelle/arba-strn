#include <strn/string64.hpp>

namespace strn
{
string64::string64(const std::string_view& str)
    : string64()
{
    std::copy(str.begin(), str.begin() + std::min<std::size_t>(buffer_size_, str.length()), cstr_.begin());
}

string64::const_iterator string64::end() const
{
    const_iterator iter = begin();
    for (uint8_t i = 0; *iter && i < buffer_size_; ++iter, ++i);
    return iter;
}

string64::iterator string64::end()
{
    iterator iter = begin();
    for (uint8_t i = 0; *iter && i < buffer_size_; ++iter, ++i);
    return iter;
}

bool string64::is_printable() const
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

void string64::push_back(const char& ch)
{
    iterator end_iter = end();
    if (std::size_t length = end_iter - begin(); length < max_length())
        *end_iter = ch;
}

}
