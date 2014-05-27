#include <gtest/gtest.h>
#include <stub/some.hpp>

TEST(TestSome, return_value_of_some_method)
{
    stub::some s;
    EXPECT_TRUE(s.some_method());
}
