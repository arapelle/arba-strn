include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
    project_version_tests.cpp
    string32_tests.cpp
    string56_tests.cpp
    string64_tests.cpp
)
