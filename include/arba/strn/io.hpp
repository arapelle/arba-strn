#pragma once

#include <istream>
#include <ostream>

inline namespace arba
{
namespace strn
{

class string64;

std::ostream& operator<<(std::ostream& stream, const string64& str);
std::istream& operator>>(std::istream& stream, string64& str);

class string56;

std::ostream& operator<<(std::ostream& stream, const string56& str);
std::istream& operator>>(std::istream& stream, string56& str);

class string32;

std::ostream& operator<<(std::ostream& stream, const string32& str);
std::istream& operator>>(std::istream& stream, string32& str);

} // namespace strn
} // namespace arba
