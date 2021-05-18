// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/compare_argument.hpp>

#include <gtest/gtest.h>

TEST(test_compare_argument, basic)
{
    EXPECT_TRUE(stub::compare_argument(5, 5));
    EXPECT_FALSE(stub::compare_argument(5, 6));

    EXPECT_TRUE(stub::compare_argument(true, true));
    EXPECT_FALSE(stub::compare_argument(true, false));
}

namespace
{
struct cup
{
    double m_volume;
};
}

TEST(test_compare_argument, compare)
{
    auto cmp = stub::make_compare(
        [](const cup& c) -> bool { return c.m_volume == 2.3; });

    EXPECT_TRUE(stub::compare_argument(cup{2.3}, cmp));
    EXPECT_FALSE(stub::compare_argument(cup{2.4}, cmp));
}

TEST(test_compare_argument, ignore)
{
    EXPECT_TRUE(stub::compare_argument(cup{2.3}, stub::ignore()));
    EXPECT_TRUE(stub::compare_argument(true, stub::ignore()));
}

TEST(test_compare_argument, not_nullptr)
{
    EXPECT_FALSE(stub::compare_argument(nullptr, stub::not_nullptr()));

    std::vector<uint8_t> buffer(1);
    EXPECT_NE(nullptr, buffer.data());
    EXPECT_TRUE(stub::compare_argument(buffer.data(), stub::not_nullptr()));
    EXPECT_TRUE(stub::compare_argument("", stub::not_nullptr()));
    EXPECT_TRUE(stub::compare_argument("hello", stub::not_nullptr()));
}

TEST(test_compare_argument, string)
{
    std::string str = "hello";
    EXPECT_TRUE(stub::compare_argument(str, "hello"));
}
