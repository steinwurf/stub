// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/compare_call.hpp>

#include <gtest/gtest.h>

TEST(test_compare_call, same_type)
{
    stub::compare_call<int, bool, double> expect(5, true, 4.0);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}

TEST(test_compare_call, argument_types)
{
    stub::compare_call<int, const bool&, double&> expect(5, true, 4.0);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}


TEST(test_compare_call, store_in_vector)
{
    using expect = stub::compare_call<int, bool, double>;
    std::vector<expect> expectations;

    expectations.emplace_back(4, true, 4.0);
    expectations.emplace_back(5, true, 4.0);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_TRUE(expectations[0].compare(t1));
    EXPECT_FALSE(expectations[1].compare(t1));
}

TEST(test_compare_call, ignore)
{
    stub::compare_call<int, bool, double> expect(5, true, stub::ignore());

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}
