
#include <cstdlib>
#include <iostream>
#include <strn/version.hpp>
#include <strn/string64.hpp>
#include <strn/io.hpp>

int main()
{
    strn::string64 str("hi!");
    std::cout << str << std::endl;
    return EXIT_SUCCESS;
}
