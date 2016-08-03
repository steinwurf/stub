// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/compare_arguments.hpp>

#include <gtest/gtest.h>

TEST(test_compare_arguments, api)
{
    auto t1 = std::make_tuple(5, true, false);
    auto t2 = std::make_tuple(5, true, false);

    EXPECT_TRUE(stub::compare_arguments(t1, t2));

    auto t3 = std::make_tuple(5, false, false);

    EXPECT_FALSE(stub::compare_arguments(t1, t3));
}
