cmake_minimum_required(VERSION 3.26)

# EXTERNAL DEPENDENCIES

add_subdirectory(external)

# CMAKE INCLUDES

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/cmake/)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

# C++ PROJECT

set_project_name(NAMESPACE "arba" BASE_NAME "strn")
string(TIMESTAMP configure_datetime "%Y%m%d-%H%M%S")
set_project_semantic_version("0.5.0" PRE_RELEASE "dev" BUILD_METADATA "${configure_datetime}")

project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "A C++ library providing short trivially comparable string classes whose hashs are trivially computable."
        HOMEPAGE_URL "https://github.com/arapelle/arba-strn"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_SEMANTIC_VERSION} ${CMAKE_BUILD_TYPE}")

# C++ LIBRARY

## Generated/Configured headers:
configure_headers(configured_headers
    FILES
        include/${PROJECT_NAMESPACE}/${PROJECT_BASE_NAME}/version.hpp.in
)

## Headers:
set(headers
    include/arba/strn/c_str_traits.hpp
    include/arba/strn/io.hpp
    include/arba/strn/string32.hpp
    include/arba/strn/string56.hpp
    include/arba/strn/string64.hpp
    include/arba/strn/string_n_helper.hpp
    include/arba/strn/string_tokenizer.hpp
)

## Sources:
set(sources
    src/io.cpp
    src/string32.cpp
    src/string56.cpp
    src/string64.cpp
)

## Add C++ library:
shared_or_static_option(${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE "SHARED")
if("${${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE}" STREQUAL "SHARED")
  set(LIBRARY_TYPE_POSTFIX "")
  set(PROJECT_TARGET_NAME "${PROJECT_NAME}")
else()
  set(LIBRARY_TYPE_POSTFIX "-static")
  set(PROJECT_TARGET_NAME "${PROJECT_NAME}${LIBRARY_TYPE_POSTFIX}")
endif()

add_cpp_library(${PROJECT_TARGET_NAME} ${${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE}
  HEADERS ${headers} ${configured_headers}
  SOURCES ${sources}
  HEADERS_BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  CXX_STANDARD 20
  DEFAULT_WARNING_OPTIONS
)
add_library("${PROJECT_NAMESPACE}::${PROJECT_BASE_NAME}${LIBRARY_TYPE_POSTFIX}" ALIAS ${PROJECT_TARGET_NAME})

## Link C++ targets:
find_package(arba-cppx 0.1.0 REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        arba::cppx
)

## Add tests:
add_test_subdirectory_if_build(test)

## Add examples:
add_example_subdirectory_if_build(example)

# C++ INSTALL

## Install C++ library:
install_cpp_libraries(TARGETS ${PROJECT_TARGET_NAME} EXPORT ${PROJECT_NAME}-targets)

## Install project package
install_library_package(${PROJECT_NAME} INPUT_PACKAGE_CONFIG_FILE cmake/config/package-config.cmake.in)
install_uninstall_script(${PROJECT_NAME})
