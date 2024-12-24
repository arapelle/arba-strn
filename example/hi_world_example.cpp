#include <arba/strn/io.hpp>
#include <arba/strn/string64.hpp>
#include <iostream>

int main()
{
    strn::string64 str("Hi world");
    std::cout << str << std::endl;
    return EXIT_SUCCESS;
}
