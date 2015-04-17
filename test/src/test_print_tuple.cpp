// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/print_tuple.hpp>

#include <gtest/gtest.h>

TEST(print_tuple, empty_tuple)
{
    std::stringstream stream;

    auto t = std::make_tuple();
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), std::string(""));

}

TEST(print_tuple, non_empty_tuple)
{
    std::stringstream stream;

    auto t = std::make_tuple(43U, true, 3.14);
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), std::string("Arg 0: 43\nArg 1: 1\nArg 2: 3.14\n"));
}

namespace stub_testing
{
    struct my_type
    {
        my_type() : m_string("okok"), m_value(42U) {}

        std::string m_string;
        uint32_t m_value;
    };

    std::ostream& operator<<(std::ostream& out, const my_type& t)
    {
        out << t.m_string << t.m_value;
        return out;
    }
}

TEST(print_tuple, user_defined_types)
{
    std::stringstream stream;

    auto t = std::make_tuple(43U, stub_testing::my_type());
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), std::string("Arg 0: 43\nArg 1: okok42\n"));
}
