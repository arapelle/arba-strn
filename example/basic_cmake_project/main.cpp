#include <cstdlib>
#include <iostream>
#include <arba/strn/version.hpp>
#include <arba/strn/string64.hpp>
#include <arba/strn/io.hpp>

int main()
{
    arba::strn::string64 str("hi!");
    strn::string64 str2("hello!");
    std::cout << str << std::endl;
    std::cout << str2 << std::endl;
    return EXIT_SUCCESS;
}
