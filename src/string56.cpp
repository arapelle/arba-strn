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

void string56::push_back(const char& ch)
{
    if (std::size_t length = this->length(); length < max_length())
    {
        *end() = ch;
        ++cstr_.back();
    }
}

void string56::pop_back()
{
    iterator end_iter = end();
    *(--end_iter) = 0;
    --cstr_.back();
}

void string56::resize(std::size_t new_length, char new_ch)
{
    new_length = std::min(new_length, max_length());
    if (new_length < length())
        std::fill(begin() + new_length, end(), 0);
    else
        std::fill(end(), begin() + new_length, new_ch);
    cstr_.back() = new_length;
}

}
