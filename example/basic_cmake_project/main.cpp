#include <cstdlib>
#include <iostream>
#include <arba/strn/version.hpp>
#include <arba/strn/string64.hpp>
#include <arba/strn/io.hpp>

int main()
{
    strn::string64 str("hi!");
    std::cout << str << std::endl;
    return EXIT_SUCCESS;
}
