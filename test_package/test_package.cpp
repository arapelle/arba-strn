#include <arba/strn/io.hpp>
#include <arba/strn/string64.hpp>
#include <arba/strn/version.hpp>
#include <iostream>
#include <unordered_map>

int main()
{
    std::unordered_map<arba::strn::string64, std::string> dict;
    dict["id1"] = "A long string";
    dict["id2"] = "Another long string";
    dict["12345678"] = "'12345678' length is string64 max length.";
    std::cout << dict["id1"] << std::endl << std::endl;
    for (const auto& entry : dict)
        std::cout << entry.first << ": " << entry.second << std::endl;
    std::cout << "TEST PACKAGE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
