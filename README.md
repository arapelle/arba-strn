# Concept #

The purpose is to provide short string classes (8 bytes max) whose hashs are trivially computable
(no loop operation), and comparison operations are trivial too (no loop operation again).

# Install #
## Requirements ##

Binaries:
- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):
- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-strn --recurse-submodules
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-strn/0.5.0")
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-strn
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-strn/
cmake -P uninstall.cmake
```

# How to use
## Example - "Hi world"
```c++
#include <arba/strn/string64.hpp>
#include <arba/strn/io.hpp>
#include <iostream>

int main()
{
    strn::string64 str("Hi world");
    std::cout << str << std::endl;
    return EXIT_SUCCESS;
}
```

## Example - As a map key
```c++
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

# License

[MIT License](./LICENSE.md) © arba-strn
