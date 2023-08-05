cmake_minimum_required(VERSION 3.26)

include(CMakePrintHelpers)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}-static main.cpp)
# Find package arba-strn:
find_package(arba-strn 0.2.0 CONFIG REQUIRED)
# Use arba-strn release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba-strn)
# Use arba-strn release static target:
target_link_libraries(${PROJECT_NAME}-static PRIVATE arba-strn-static)
