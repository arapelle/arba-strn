#include <strn/string56.hpp>

namespace strn
{
string56::string56(const std::string_view& str)
    : string56()
{
    std::size_t len = std::min<std::size_t>(max_length(), str.length());
    std::copy(str.begin(), str.begin() + len, cstr_.begin());
    cstr_.back() = len;
}

bool string56::is_printable() const
{
    for (buffer_type_::const_iterator iter = cstr_.begin(), end_iter = end();
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

}
