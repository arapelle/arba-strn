cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

# Find package strn:
find_package(strn 0.1.5 CONFIG REQUIRED)

# Here, strn is used as a PRIVATE dependency
add_executable(${PROJECT_NAME} main.cpp)
# Use strn release shared library:
target_link_libraries(${PROJECT_NAME} PRIVATE arba::strn)
# Use strn release static library:
#target_link_libraries(${PROJECT_NAME} PRIVATE arba::strn-static)
