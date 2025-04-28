cmake_minimum_required(VERSION 3.15)

project(PackageTest CXX)

add_executable(test_package
    test_package.cpp
)

set(lib_namespace "arba")
set(lib_feature_name "strn")
find_package(${lib_namespace}-${lib_feature_name} CONFIG REQUIRED)

if(TARGET ${lib_namespace}::${lib_feature_name})
    set(lib_target "${lib_namespace}::${lib_feature_name}")
else()
    set(lib_target "${lib_namespace}::${lib_feature_name}-static")
endif()
target_link_libraries(test_package
    PRIVATE
        ${lib_target}
)

target_compile_features(test_package PRIVATE cxx_std_20)
