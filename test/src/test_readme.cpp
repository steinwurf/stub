// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/function.hpp>

#include <gtest/gtest.h>

/// Tests that all the examples given in the readme works as advertised
TEST(test_readme, check_a_set_of_function_calls)
{
    stub::function<void(uint32_t)> some_function;

    some_function(3);
    some_function(4);

    // Expectation matches how we called the function
    bool works = some_function.expect_calls()
        .with(3U)
        .with(4U)
        .to_bool();

    assert(works == true);

    // Not the right call order
    works = some_function.expect_calls()
        .with(4U)
        .with(3U)
        .to_bool();

    assert(works == false);

}

TEST(test_readme, function_taking_no_arguments)
{
    stub::function<void()> function;
    function();
    function();

    // Is matched by:
    bool works = function.expect_calls()
        .with()
        .with()
        .to_bool();

    assert(works);
}

TEST(test_readme, check_the_number_of_function_calls)
{
    stub::function<void(uint32_t)> some_function;

    some_function(3);
    some_function(4);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    // Return true if no calls were made
    assert(some_function.no_calls() == false);
}

TEST(test_readme, get_the_arguments_of_a_specific_call)
{
    stub::function<void(uint32_t,uint32_t)> function;

    function(3,4);
    function(4,3);
    function(2,6);

    auto a = function.call_arguments(1);
    auto b = std::make_tuple(4,3);

    assert(a == b);
}

struct cup
{
    double m_volume;
};




TEST(test_readme, comparing_custom_arguments)
{
    {
        stub::function<void(const cup&)> function;

        function(cup{2.3});
        function(cup{4.5});

        auto compare = [](double expected, const cup& c)-> bool
        { return c.m_volume == expected; };

        assert(function.expect_calls()
            .with(stub::make_compare(
                std::bind(compare, 2.3, std::placeholders::_1)))
            .with(stub::make_compare(
                std::bind(compare, 4.5, std::placeholders::_1)))
            .to_bool());
    }

    {
    using element = std::pair<uint32_t, uint32_t>;

    auto expect = [](uint32_t expected, const element& actual) -> bool
        { return expected == actual.second; };

    stub::function<void(const element&)> function;
    function(element(2,3));
    function(element(20,3));

    // We have called the function more than once
    assert(false == function.expect_calls()
        .with(stub::make_compare(
            std::bind(expect, 3, std::placeholders::_1))).to_bool());

    // Works since we only match the second value of the pair
    assert(true == function.expect_calls()
        .with(stub::make_compare(
            std::bind(expect, 3, std::placeholders::_1)))
        .with(stub::make_compare(
            std::bind(expect, 3, std::placeholders::_1))).to_bool());

    // Without the custom comparison it fails
    assert(false == function.expect_calls()
        .with(element(1,3))
        .with(element(2,3)).to_bool());
    }
}
