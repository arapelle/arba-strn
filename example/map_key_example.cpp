#include <arba/strn/string64.hpp>
#include <arba/strn/io.hpp>
#include <unordered_map>
#include <iostream>

int main()
{
    std::unordered_map<strn::string64, std::string> dict;
    dict["id1"] = "A long string";
    dict["id2"] = "Another long string";
    dict["12345678"] = "'12345678' length is string64 max length.";
//    dict["123456789"] = "It cannot compile as only 8-length or shorter C-string are accepted.";
    std::cout << dict["id1"] << std::endl << std::endl;
    for (const auto& entry : dict)
        std::cout << entry.first << ": " << entry.second << std::endl;
    return EXIT_SUCCESS;
}
