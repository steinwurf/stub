// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/expect_arguments.hpp>

#include <gtest/gtest.h>

TEST(test_expect_arguments, same_type)
{
    auto expectation = std::make_tuple(5, true, 4.0);
    stub::expect_arguments<int, bool, double> expect(expectation);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}

TEST(test_expect_arguments, store_in_vector)
{
    using expect = stub::expect_arguments<int, bool, double>;
    std::vector<expect> expectations;

    expectations.push_back(std::make_tuple(4, true, 4.0));
    expectations.push_back(std::make_tuple(5, true, 4.0));

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_TRUE(expectations[0].compare(t1));
    EXPECT_FALSE(expectations[1].compare(t1));
}

/*TEST(test_expect_arguments, ignore)
{
    auto expectation = std::make_tuple(5, true, 4.0);
    stub::expect_arguments<int, bool, double> expect(expectation);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}
*/
