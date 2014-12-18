// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/version.hpp>

#include <gtest/gtest.h>

TEST(TestVersion, not_empty)
{
    EXPECT_FALSE(gtest::version().empty());
}
