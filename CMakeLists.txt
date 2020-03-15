cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# CMake includes
include(CMakePrintHelpers)
# Project include
include(Utility)
include(CppLibraryProject)

check_cmake_binary_dir()

#-----

# Build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# PROJECT

project(strn
    VERSION 0.1.0
    DESCRIPTION "A C++ library providing short trivially comparable string classes whose hash are trivially computable."
    HOMEPAGE_URL "https://github.com/arapelle/strn/tree/master/include/strn"
    LANGUAGES CXX)

#-----
## C++ PROJECT CONF

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#-----
## PROJECT CONF

# OPTIONS
option(${PROJECT_NAME}_BUILD_SHARED_LIB "Indicates if we build a SHARED library." ON)
option(${PROJECT_NAME}_BUILD_STATIC_LIB "Indicates if we build a STATIC library." ON)
if(EXISTS "${PROJECT_SOURCE_DIR}/test/CMakeLists.txt")
    option(${PROJECT_NAME}_BUILD_TESTS "Indicates if we build the tests or not." OFF)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/example/CMakeLists.txt")
    option(${PROJECT_NAME}_BUILD_EXAMPLES "Indicates if we build the examples or not." OFF)
endif()

# OUTPUT PATHS
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib)

# PRINT STATUS
message(STATUS "PROJECT : ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "BUILD   : ${CMAKE_BUILD_TYPE}")
message(STATUS "CPPCOMP : ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")


if(NOT ${${PROJECT_NAME}_BUILD_SHARED_LIB} AND NOT ${${PROJECT_NAME}_BUILD_STATIC_LIB})
    message(FATAL_ERROR "You did not choose which target(s) to build (SHARED, STATIC).")
endif()

#-----

# GENERATE version.hpp
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version/version.hpp.in"
               "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/version.hpp")

#-----

include(GNUInstallDirs)

# TARGET LIBRARY
set(project_object_target ${PROJECT_NAME}-object)
set(project_shared_target ${PROJECT_NAME})
set(project_static_target ${PROJECT_NAME}-static)
set(export_name ${PROJECT_NAME})

# add library
file(GLOB_RECURSE target_header_files include/*)
file(GLOB_RECURSE target_src_files src/*)
add_library(${project_object_target} OBJECT ${target_header_files} ${target_src_files})
target_include_directories(${project_object_target} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_property(TARGET ${project_object_target} PROPERTY POSITION_INDEPENDENT_CODE 1)
# compile options
if(MSVC)
    target_compile_options(${project_object_target} PRIVATE /Wall)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${project_object_target} PRIVATE -Wall -Wextra -pedantic)
endif()

if(${${PROJECT_NAME}_BUILD_SHARED_LIB})
    add_library(${project_shared_target} SHARED $<TARGET_OBJECTS:${project_object_target}>)
    target_include_directories(${project_shared_target} PUBLIC $<INSTALL_INTERFACE:include>)
    set_target_properties(${project_shared_target} PROPERTIES DEBUG_POSTFIX "-d" SOVERSION ${PROJECT_VERSION})
    set(project_targets ${project_targets} ${project_shared_target})
endif()

if(${${PROJECT_NAME}_BUILD_STATIC_LIB})
    add_library(${project_static_target} STATIC $<TARGET_OBJECTS:${project_object_target}>)
    target_include_directories(${project_static_target} PUBLIC $<INSTALL_INTERFACE:include>)
    set_target_properties(${project_static_target} PROPERTIES DEBUG_POSTFIX "-d")
    set(project_targets ${project_targets} ${project_static_target})
endif()

#-----

# INSTALL
install(TARGETS ${project_targets} EXPORT ${export_name})
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME} DESTINATION include)
install(EXPORT ${export_name} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

set(install_cmake_package_dir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
configure_package_config_file(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-config.cmake.in
                             "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
                             INSTALL_DESTINATION ${install_cmake_package_dir}
                             NO_SET_AND_CHECK_MACRO
                             NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/strn-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/strn-config-version.cmake
    DESTINATION ${install_cmake_package_dir})

#-----

# EXAMPLES
if(EXISTS "${PROJECT_SOURCE_DIR}/example/CMakeLists.txt" AND ${${PROJECT_NAME}_BUILD_EXAMPLES})
    add_subdirectory(example)
endif()

# TESTS
if(EXISTS "${PROJECT_SOURCE_DIR}/test/CMakeLists.txt" AND ${${PROJECT_NAME}_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()

install(CODE
    "
    message(STATUS \"Installing: ${install_cmake_package_dir}/cmake_uninstall.cmake\")
    if(DEFINED CMAKE_INSTALL_MANIFEST_FILES)
        set(files \${CMAKE_INSTALL_MANIFEST_FILES} ${install_cmake_package_dir}/cmake_uninstall.cmake)
        file(APPEND ${install_cmake_package_dir}/cmake_uninstall.cmake
        \"
        message(STATUS \\\"Uninstall ${PROJECT_NAME} v${PROJECT_VERSION} ${CMAKE_BUILD_TYPE}\\\")
        foreach(file \${files})
            while(NOT \\\${file} STREQUAL \${CMAKE_INSTALL_PREFIX})
                if(EXISTS \\\${file} OR IS_SYMLINK \\\${file})
                    if(IS_DIRECTORY \\\${file})
                        file(GLOB dir_files \\\${file}/*)
                        list(LENGTH dir_files number_of_files)
                        if(\\\${number_of_files} EQUAL 0)
                          message(STATUS \\\"Removing  dir: \\\${file}\\\")
                          file(REMOVE_RECURSE \\\${file})
                        endif()
                    else()
                        message(STATUS \\\"Removing file: \\\${file}\\\")
                        file(REMOVE \\\${file})
                    endif()
                endif()
                get_filename_component(file \\\${file} DIRECTORY)
            endwhile()
        endforeach()
        \"
        )
    else()
        message(ERROR \"cmake_uninstall.cmake script cannot be created!\")
    endif()
 "
)

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
