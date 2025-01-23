#include <arba/strn/version.hpp>
#include <gtest/gtest.h>

TEST(project_version_tests, test_version_core)
{
    constexpr unsigned major = 0;
    constexpr unsigned minor = 4;
    constexpr unsigned patch = 1;
    static_assert(arba::strn::version.core() == arba::vrsn::numver(major, minor, patch));
}
