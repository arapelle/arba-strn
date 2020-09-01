cmake_minimum_required(VERSION 3.16)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

# Standard includes
include(CMakePrintHelpers)
# Custom include
include(cmtk/CppLibraryProject)

#-----
# PROJECT

check_cmake_binary_dir()
set_build_type_if_undefined()

#-----
# C++ PROJECT

project(strn
        VERSION 0.1.3
        DESCRIPTION "A C++ library providing short trivially comparable string classes whose hash are trivially computable."
        HOMEPAGE_URL "https://github.com/arapelle/strn"
        LANGUAGES CXX)

message(STATUS "BUILD   : ${CMAKE_BUILD_TYPE}")
message(STATUS "CPPCOMP : ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_VERSION}")

#-----
include(CTest)

# Project options
library_build_options(${PROJECT_NAME} STATIC SHARED EXAMPLE TEST)

# Headers:
set(headers
    include/strn/c_str_traits.hpp
    include/strn/io.hpp
    include/strn/string32.hpp
    include/strn/string56.hpp
    include/strn/string64.hpp
)

# Sources:
set(sources
    src/io.cpp
    src/string32.cpp
    src/string56.cpp
    src/string64.cpp
)

# Add C++ library
add_cpp_library(${PROJECT_NAME} ${PROJECT_NAME}_BUILD_SHARED_LIB ${PROJECT_NAME}_BUILD_STATIC_LIB
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    CXX_STANDARD 20
    INCLUDE_DIRECTORIES include
    OUTPUT_VERSION_HEADER "version.hpp"
    HEADERS ${headers}
    SOURCES ${sources}
    BUILT_TARGETS project_targets
    )

# Install C++ library
install_cpp_library_targets(${PROJECT_NAME}
                            TARGETS ${project_targets}
                            INCLUDE_DIRECTORIES "include/${PROJECT_NAME}"
                            NAMESPACE strn::
                            )

# Install package
install_package(${PROJECT_NAME}
                VERBOSE_PACKAGE_CONFIG_FILE
                EXPORT_NAMES ${PROJECT_NAME})

if(${PROJECT_NAME}_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(${PROJECT_NAME}_BUILD_TESTS AND BUILD_TESTING)
    add_subdirectory(test)
endif()

#-----

# PACKAGE
set(CPACK_GENERATOR                   "STGZ;TGZ;ZIP")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR              "Aymeric Pellé")
set(CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE       "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR       "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR       "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH       "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CMAKE_PROJECT_NAME}")
#if(WIN32 AND NOT UNIX)
#  set(CPACK_NSIS_DISPLAY_NAME         "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
#  set(CPACK_NSIS_CONTACT              "arapelle@gmail.com")
#  set(CPACK_NSIS_MODIFY_PATH ON)
#else(WIN32 AND NOT UNIX)
#endif(WIN32 AND NOT UNIX)

include(CPack)
