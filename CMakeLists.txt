cmake_minimum_required(VERSION 3.8)

# PROJECT
project(strn CXX)

# version numbers
set(STRN_VERSION_MAJOR 0)
set(STRN_VERSION_MINOR 1)
set(STRN_VERSION_PATCH 0)
set(STRN_VERSION_COMMENT "strn::string64 created.")

#-----

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#-----

# STRN CONFIGURATION
option(STRN_BUILD_SHARED_LIB "Indicates if we build a SHARED library or a STATIC library." ON)
option(STRN_BUILD_TESTS "Indicates if we build the tests or not." OFF)

#-----

# OUTPUT
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_TYPE}/lib)

# INCLUDE/SRC
set(INCLUDE_DIRECTORY include)
set(SRC_DIRECTORY src)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version/version.hpp.in"
  "${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}/${PROJECT_NAME}/version.hpp"
)

# INCLUDE
include_directories(${INCLUDE_DIRECTORY})
include_directories(${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY})

message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE})

#-----

# STRN LIBRARY
set(target_name ${PROJECT_NAME})

# find source files and add library
file(GLOB_RECURSE target_src_files ${SRC_DIRECTORY}/*)
if(${STRN_BUILD_SHARED_LIB})
    add_library(${target_name} SHARED ${target_src_files})
else()
    add_library(${target_name} STATIC ${target_src_files})
endif()

# compile options
if(MSVC)
    target_compile_options(${target_name} PRIVATE /Wall)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -pedantic)
endif()

# install
install(TARGETS ${target_name})
install(DIRECTORY ${INCLUDE_DIRECTORY}/${PROJECT_NAME} DESTINATION ${INCLUDE_DIRECTORY})
install(DIRECTORY ${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}/${PROJECT_NAME} DESTINATION ${INCLUDE_DIRECTORY})

#-----

# TESTS
if(${STRN_BUILD_TESTS})
    enable_testing()
    set(TEST_DIRECTORY test)
    set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/test)
    file(GLOB tests_cpp_files "${TEST_DIRECTORY}/*.cpp")
    foreach(filename ${tests_cpp_files})
        get_filename_component(test_exe_name ${filename} NAME_WE)
        add_executable(${test_exe_name} ${filename})
        target_link_libraries(${test_exe_name} strn)
        add_test("${test_exe_name}" ${EXECUTABLE_OUTPUT_PATH}/${test_exe_name})
    endforeach()
endif()

#-----

# PACKAGE

set(CPACK_GENERATOR                   "STGZ;TGZ;ZIP")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR              "Aymeric Pellé")
set(CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE       "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR       "${STRN_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR       "${STRN_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH       "${STRN_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CMAKE_PROJECT_NAME}")
#if(WIN32 AND NOT UNIX)
#  set(CPACK_NSIS_DISPLAY_NAME         "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
#  set(CPACK_NSIS_CONTACT              "arapelle@gmail.com")
#  set(CPACK_NSIS_MODIFY_PATH ON)
#else(WIN32 AND NOT UNIX)
#endif(WIN32 AND NOT UNIX)

include(CPack)
