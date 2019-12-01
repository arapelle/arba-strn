# Concept #

The purpose is to provide a short string class (8 bytes max) whose hash is trivially computable
(no loop operation), and comparaison operations are trivial too (no loop operation again).

# Install #
## Build ##
You need a C++17 compiler (ex: g++-9).
```
cd /path/to/strn
mkdir build
cd build
cmake ..
make
```

## Install ##
`make install`

## Create a package ##
`cpack`

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

## Includes
* `<strn/string64.hpp>`
    * class `strn::string64`.
    * string literal `_s64`.
    * class `std::hash<strn::string64>`.
* `<strn/c_str_traits.hpp>`
    * C-string traits.
* `<strn/hash.hpp>`
    * string literal `_hash`. (ex: `case "val"_hash:`)

## Construct a string64

* Default constructor
```c++
strn::string64 str64;
```
* Constructor with a C-string
```c++
strn::string64 str64("12345678");
```
* Constructor with a C++ string
```c++
std::string str("test");
strn::string64 str64(str);
```
* Constructor with a C++ string view
```c++
std::string str("test");
std::string_view strv(str);
strn::string64 str64(strv);
```
* Construct from a C-string
```c++
auto str64 = "value"_s64;
```

## Member functions of string64
```c++
constexpr const uint&      integer() const;
constexpr std::size_t      hash() const;
          std::string_view to_string_view() const;
          std::string      to_string() const;
          bool             empty() const;
          bool             not_empty() const;
          std::size_t      length() const;
          const_iterator   begin() const;
          iterator         begin();
          const_iterator   end() const;
          iterator         end();
          const_iterator   cbegin() const;
          const_iterator   cend() const;
          const char&      operator[](std::size_t index) const;
          char&            operator[](std::size_t index);
constexpr bool             operator==(const string64& rhs) const;
constexpr bool             operator!=(const string64& rhs) const;
constexpr bool             operator< (const string64& rhs) const;
```
## Static functions of string64
```c++
constexpr static std::size_t max_length();
```
