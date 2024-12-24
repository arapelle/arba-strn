#include <arba/strn/string32.hpp>

inline namespace arba
{
namespace strn
{

bool string32::is_printable() const
{
    for (buffer_type_::const_iterator iter = cstr_.begin(), end_iter = end(); iter != end_iter; ++iter)
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

void string32::pop_back()
{
    iterator end_iter = end();
    *(--end_iter) = 0;
}

void string32::resize(std::size_t new_length, char new_ch)
{
    new_length = std::min(new_length, max_length());
    auto iter = begin(), end_iter = begin() + new_length;
    // Parsing until of new string end reached or null character found.
    for (; iter != end_iter && *iter != 0; ++iter)
        ;
    // Add new characters if the new size is greater.
    for (; iter != end_iter; ++iter)
        *iter = new_ch;
    // The rest of the buffer is set to 0.
    for (end_iter = begin() + max_length(); iter != end_iter; ++iter)
        *iter = 0;
}

} // namespace strn
} // namespace arba
