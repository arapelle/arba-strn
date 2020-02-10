cmake_minimum_required(VERSION 3.8)

include(CMakePrintHelpers)

# CHECK CMAKE_BINARY_DIR
set(bin_dir ${CMAKE_BINARY_DIR})
set(bin_dir_while TRUE)
while(${bin_dir_while})
    if(${bin_dir} STREQUAL ${CMAKE_SOURCE_DIR})
        message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
        message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
        message(FATAL_ERROR "CMAKE_BINARY_DIR must not be located in CMAKE_SOURCE_DIR!")
    endif()
    get_filename_component(parent_bin_dir ${bin_dir} DIRECTORY)
    if(${parent_bin_dir} STREQUAL ${bin_dir})
        set(bin_dir_while FALSE)
    else()
        set(bin_dir ${parent_bin_dir})
    endif()
endwhile()
unset(parent_bin_dir)
unset(bin_dir_while)
unset(bin_dir)

# PROJECT
project(strn CXX)

#-----
## C++ PROJECT CONF

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
## STRN PROJECT CONF

# VERSION
set(STRN_VERSION_MAJOR 0)
set(STRN_VERSION_MINOR 1)
set(STRN_VERSION_PATCH 0)
set(STRN_VERSION ${STRN_VERSION_MAJOR}.${STRN_VERSION_MINOR}.${STRN_VERSION_PATCH})
set(STRN_VERSION_COMMENT "strn::string64 created.")

# OPTIONS
option(STRN_BUILD_SHARED_LIB "Indicates if we build a SHARED library." ON)
option(STRN_BUILD_STATIC_LIB "Indicates if we build a STATIC library." ON)
option(STRN_BUILD_TESTS "Indicates if we build the tests or not." OFF)
option(STRN_BUILD_EXAMPLES "Indicates if we build the examples or not." OFF)

# OUTPUT PATHS
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib)

# PRINT STATUS
message(STATUS "PROJECT: ${PROJECT_NAME} v${STRN_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

#-----

# SET VARS INCLUDE/SRC
set(INCLUDE_DIRECTORY include)
set(SRC_DIRECTORY src)

# GENERATE version.hpp
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version/version.hpp.in"
               "${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}/${PROJECT_NAME}/version.hpp")

#-----

include(GNUInstallDirs)

# TARGET LIBRARY STRN
set(strn_object_target ${PROJECT_NAME}-object)
set(strn_shared_target ${PROJECT_NAME})
set(strn_static_target ${PROJECT_NAME}-static)
set(export_name ${PROJECT_NAME})

# add library
file(GLOB_RECURSE target_header_files ${INCLUDE_DIRECTORY}/*)
file(GLOB_RECURSE target_src_files ${SRC_DIRECTORY}/*)
add_library(${strn_object_target} OBJECT ${target_header_files} ${target_src_files})
target_include_directories(${strn_object_target} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIRECTORY}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}>)
set_property(TARGET ${strn_object_target} PROPERTY POSITION_INDEPENDENT_CODE 1)
# compile options
if(MSVC)
    target_compile_options(${strn_object_target} PRIVATE /Wall)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${strn_object_target} PRIVATE -Wall -Wextra -pedantic)
endif()

if(${STRN_BUILD_SHARED_LIB})
    add_library(${strn_shared_target} SHARED $<TARGET_OBJECTS:${strn_object_target}>)
    target_include_directories(${strn_shared_target} PUBLIC $<INSTALL_INTERFACE:${INCLUDE_DIRECTORY}>)
    set_target_properties(${strn_shared_target} PROPERTIES DEBUG_POSTFIX "-d")
endif()

if(${STRN_BUILD_STATIC_LIB})
    add_library(${strn_static_target} STATIC $<TARGET_OBJECTS:${strn_object_target}>)
    target_include_directories(${strn_static_target} PUBLIC $<INSTALL_INTERFACE:${INCLUDE_DIRECTORY}>)
    set_target_properties(${strn_static_target} PROPERTIES DEBUG_POSTFIX "-d")
endif()

#-----

# INSTALL
install(TARGETS ${strn_shared_target} ${strn_static_target} EXPORT ${export_name})
install(DIRECTORY ${INCLUDE_DIRECTORY}/${PROJECT_NAME} DESTINATION ${INCLUDE_DIRECTORY})
install(DIRECTORY ${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}/${PROJECT_NAME} DESTINATION ${INCLUDE_DIRECTORY})
install(EXPORT ${export_name} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/strn-config.cmake.in
                             "${CMAKE_CURRENT_BINARY_DIR}/strn-config.cmake"
                             INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
                             NO_SET_AND_CHECK_MACRO
                             NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/strn-config-version.cmake"
    VERSION ${STRN_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/strn-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/strn-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

#-----

# EXAMPLES
if(${STRN_BUILD_EXAMPLES})
    set(PROGRAMS_DIRECTORY example)
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROGRAMS_DIRECTORY})
    file(GLOB programs_cpp_files "${PROGRAMS_DIRECTORY}/*.cpp")
    foreach(filename ${programs_cpp_files})
        get_filename_component(program_exe_name ${filename} NAME_WE)
        add_executable(${program_exe_name} ${filename})
        target_link_libraries(${program_exe_name} strn)
        target_include_directories(${program_exe_name} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIRECTORY}>
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}>)
    endforeach()
endif()

#-----

# TESTS
if(${STRN_BUILD_TESTS})
    enable_testing()
    set(PROGRAMS_DIRECTORY test)
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROGRAMS_DIRECTORY})
    file(GLOB programs_cpp_files "${PROGRAMS_DIRECTORY}/*.cpp")
    foreach(filename ${programs_cpp_files})
        get_filename_component(program_exe_name ${filename} NAME_WE)
        add_executable(${program_exe_name} ${filename})
        target_link_libraries(${program_exe_name} strn)
        target_include_directories(${program_exe_name} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIRECTORY}>
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${INCLUDE_DIRECTORY}>)
        add_test("${program_exe_name}" ${EXECUTABLE_OUTPUT_PATH}/${program_exe_name})
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
