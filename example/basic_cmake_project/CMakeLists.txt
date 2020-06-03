cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

# Find package strn:
find_package(strn 0.1.0 CONFIG REQUIRED)

# Here, strn is used as a PRIVATE dependency
add_executable(${PROJECT_NAME} main.cpp)
get_target_property(strn_compile_features strn INTERFACE_COMPILE_FEATURES)
target_compile_features(${PROJECT_NAME} PRIVATE ${strn_compile_features}) # For C++ standard
# Use strn release shared library:
get_target_property(strn_include_directories strn INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(strn_location strn IMPORTED_LOCATION_RELEASE)
# Use strn release static library:
#get_target_property(strn_include_directories strn-static INTERFACE_INCLUDE_DIRECTORIES)
#get_target_property(strn_location strn-static IMPORTED_LOCATION_RELEASE)
target_include_directories(${PROJECT_NAME} PRIVATE ${strn_include_directories})
target_link_libraries(${PROJECT_NAME} PRIVATE ${strn_location})
