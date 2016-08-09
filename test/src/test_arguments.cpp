// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/arguments.hpp>

#include <gtest/gtest.h>

/// Test that the unqualified type helper works as expected
TEST(test_arguments, test)
{
    {
        auto ok = std::is_same<std::tuple<uint32_t, uint32_t>,
            stub::arguments<const uint32_t&, uint32_t>>::value;

        EXPECT_TRUE(ok);
    }

    {
        auto ok = std::is_same<std::tuple<uint32_t, uint32_t>,
            stub::arguments<uint32_t&, uint32_t>>::value;

        EXPECT_TRUE(ok);
    }
}
