cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

# Find package strn:
find_package(strn 0.1.0 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Use strn release shared library:
target_include_directories(${PROJECT_NAME} PRIVATE $<TARGET_PROPERTY:strn,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<TARGET_PROPERTY:strn,IMPORTED_LOCATION_RELEASE>)
# Use strn release static library:
#target_include_directories(${PROJECT_NAME} PRIVATE $<TARGET_PROPERTY:strn-static,INTERFACE_INCLUDE_DIRECTORIES>)
#target_link_libraries(${PROJECT_NAME} PRIVATE $<TARGET_PROPERTY:strn-static,IMPORTED_LOCATION_RELEASE>)
