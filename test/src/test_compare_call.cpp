// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/compare_call.hpp>

#include <gtest/gtest.h>

TEST(test_compare_call, same_type)
{
    std::tuple<int,bool,double> expectation = std::make_tuple(5, true, 4.0);

    std::cout << "ok " << std::is_same<stub::arguments<int,bool,double>,std::tuple<int,bool,double>>::value << std::endl;

    stub::compare_call<int, bool, double> expect(expectation);
/*
    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));*/
}

TEST(test_compare_call, store_in_vector)
{
/*    using expect = stub::compare_call<int, bool, double>;
    std::vector<expect> expectations;

    expectations.push_back(std::make_tuple(4, true, 4.0));
    expectations.push_back(std::make_tuple(5, true, 4.0));

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_TRUE(expectations[0].compare(t1));
    EXPECT_FALSE(expectations[1].compare(t1));*/
}

/*TEST(test_compare_call, ignore)
{
    auto expectation = std::make_tuple(5, true, 4.0);
    stub::compare_call<int, bool, double> expect(expectation);

    auto t1 = std::make_tuple(4, true, 4.0);
    EXPECT_FALSE(expect.compare(t1));

    auto t2 = std::make_tuple(5, true, 4.0);
    EXPECT_TRUE(expect.compare(t2));
}
*/
