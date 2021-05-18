// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/make_compare.hpp>

#include <gtest/gtest.h>

TEST(test_make_compare, basic)
{
    auto compare =
        stub::make_compare([](uint32_t v) -> bool { return v == 1U; });

    EXPECT_TRUE(compare(1U));
    EXPECT_FALSE(compare(2U));
}
