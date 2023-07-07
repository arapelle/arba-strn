
find_or_fetch_GTest()

add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        string32_tests.cpp
        string56_tests.cpp
        string64_tests.cpp
)
