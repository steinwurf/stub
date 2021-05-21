// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/print_arguments.hpp>

#include <gtest/gtest.h>

TEST(test_print_arguments, empty_arguments)
{
    std::stringstream stream;

    auto t = std::make_tuple();
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), std::string(""));
}

TEST(test_print_arguments, non_empty_arguments)
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
    my_type() : m_string("okok"), m_value(42U)
    {
    }

    std::string m_string;
    uint32_t m_value;
};

std::ostream& operator<<(std::ostream& out, const my_type& t)
{
    out << t.m_string << t.m_value;
    return out;
}
}

TEST(test_print_arguments, user_defined_types)
{
    std::stringstream stream;

    auto t = std::make_tuple(43U, stub_testing::my_type());
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), "Arg 0: 43\nArg 1: okok42\n");
}

TEST(test_print_arguments, pointers)
{
    std::stringstream stream;

    char* p1 = (char*)0xdead1111;
    uint8_t* p2 = (uint8_t*)0xdead2222;
    bool* p3 = (bool*)0xdead3333;
    uint16_t* p4 = (uint16_t*)0xdead4444;
    stub_testing::my_type* p5 = (stub_testing::my_type*)0xdead5555;

    auto t = std::make_tuple(p1, p2, p3, p4, p5);
    stub::print_arguments(stream, t);

    EXPECT_EQ(stream.str(), "Arg 0: 0xdead1111\nArg 1: 0xdead2222\n"
                            "Arg 2: 0xdead3333\nArg 3: 0xdead4444\n"
                            "Arg 4: 0xdead5555\n");
}
