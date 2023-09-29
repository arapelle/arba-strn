#include <arba/strn/string56.hpp>

inline namespace arba
{
namespace strn
{

bool string56::is_printable() const
{
    for (buffer_type_::const_iterator iter = cstr_.begin(), end_iter = end();
         iter != end_iter;
         ++iter)
    {
        char ch = *iter;
        if (ch == 0)
            return true;
        if (!isprint(ch))
            return false;
    }
    return true;
}

void string56::push_back(const char& ch)
{
    iterator end_iter = end();
    if (auto& length = cstr_.back(); static_cast<std::size_t>(length) < max_length())
    {
        *end_iter = ch;
        ++length;
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
    auto iter = begin(), end_iter = begin() + new_length;
    // Parsing until of new string end reached or null character found.
    for (; iter != end_iter && *iter != 0; ++iter);
    // Add new characters if the new size is greater.
    for (; iter != end_iter; ++iter)
        *iter = new_ch;
    // The rest of the buffer is set to 0.
    for (end_iter = begin() + max_length(); iter != end_iter; ++iter)
        *iter = 0;
    cstr_.back() = new_length;
}

}
}
