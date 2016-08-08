// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/compare.hpp>

#include <gtest/gtest.h>

namespace
{
    bool test(uint32_t v)
    {
        return v == 1U;
    }
}

TEST(test_compare, basic)
{
    stub::compare<bool(*)(uint32_t)> compare = { test };
    EXPECT_TRUE(compare(1U));
    EXPECT_FALSE(compare(2U));
}
