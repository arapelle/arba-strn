# Concept #

The purpose is to provide a short string class (8 bytes max) whose hash is trivially computable
(no loop operation), and comparison operations are trivial too (no loop operation again).

See [wiki](https://github.com/arapelle/strn/wiki) for more details.

See [task board](https://app.gitkraken.com/glo/board/Xn4X4e25-QApB8nO) for future updates and features.

# Install #
## Requirements ##
Binaries:
- A C++17 compiler (ex: g++-9)
- CMake 3.16 or later

Libraries:
- Google Test 1.10 or later (only for testing)

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/strn
cmake -P cmake_quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -DCMAKE_BUILD_TYPE=Debug -P cmake_quick_install.cmake
```

See custom install on [wiki](https://github.com/arapelle/strn/wiki/Install#custom-install).

# How to use
## Example - "Hi world"
```c++
#include <strn/string64.hpp>
#include <strn/io.hpp>
#include <iostream>

int main()
{
    strn::string64 str("Hi world");
    std::cout << str << std::endl;
    return EXIT_SUCCESS;
}
```

To compile : `g++ example.cpp -o output -std=c++17 -lstrn`

## Example - As a map key
```c++
#include <strn/string64.hpp>
#include <strn/io.hpp>
#include <unordered_map>
#include <iostream>

int main()
{
    std::unordered_map<strn::string64, std::string> dict;
    dict["id1"] = "A long string";
    dict["id2"] = "Another long string";
    dict["12345678"] = "'12345678' length is string64 max length.";
//    dict["123456789"] = "It cannot compile as only 8-length or shorter C-string are accepted.";
    std::cout << dict["id1"] << std::endl;
    std::cout << dict["id2"] << std::endl;
    return EXIT_SUCCESS;
}
```
## Example - Using *strn* in a CMake project
See the [basic cmake project](https://github.com/arapelle/strn/tree/master/example/basic_cmake_project) example, and more specifically the [CMakeLists.txt](https://github.com/arapelle/strn/tree/master/example/basic_cmake_project/CMakeLists.txt) to see how to use *strn* in your CMake projects.

# License

[MIT License](./LICENSE.md) © strn
