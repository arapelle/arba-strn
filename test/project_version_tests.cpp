#include <arba/strn/version.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(project_version_tests, test_version_macros)
{
    ASSERT_EQ(ARBA_STRN_VERSION_MAJOR, 0);
    ASSERT_EQ(ARBA_STRN_VERSION_MINOR, 3);
    ASSERT_EQ(ARBA_STRN_VERSION_PATCH, 0);
    ASSERT_STREQ(ARBA_STRN_VERSION, "0.3.0");
}
