#pragma once

#include <istream>
#include <ostream>

namespace strn
{
class string64;

std::ostream& operator<<(std::ostream& stream, const string64& str);
std::istream& operator>>(std::istream& stream, string64& str);
}
