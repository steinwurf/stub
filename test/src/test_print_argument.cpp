// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/print_argument.hpp>

#include <gtest/gtest.h>

TEST(test_print_argument, non_pointer)
{
    std::stringstream stream;

    uint32_t v = 6;
    stub::print_argument(stream, 5, v);

    EXPECT_EQ(stream.str(), "Arg 5: 6\n");
}

TEST(test_print_argument, pointer)
{
    std::stringstream stream;

    uint8_t* v = (uint8_t*) 0xdeadbeef;
    stub::print_argument(stream, 5, v);

    EXPECT_EQ(stream.str(), "Arg 5: 0xdeadbeef\n");
}
