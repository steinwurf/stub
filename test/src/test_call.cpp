// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <stub/call.hpp>

/// Test that the call operator works as expected
TEST(call, call_operator)
{
    {
        stub::call<void()> function;
        function();

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::call<void(uint32_t)> function;
        function(2U);

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::call<void(const uint32_t&)> function;
        function(2U);

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::call<void(uint32_t&)> function;

        {
            uint32_t i = 2U;
            function(i);
        }

        EXPECT_EQ(function.calls(), 1U);

        uint32_t j = 2U;
        EXPECT_TRUE((bool)function.expect_calls().with(j));
    }

}

/// Test that the set_return(...) function works as expected
TEST(call, set_return)
{
    stub::call<uint32_t(uint32_t)> function;
    function.set_return(5U);

    EXPECT_TRUE(function.no_calls());

    auto a = function(2U);

    EXPECT_EQ(a, 5U);

    EXPECT_FALSE(function.no_calls());
    EXPECT_EQ(function.calls(), 1U);

    auto b = function(8U);

    EXPECT_EQ(b, 5U);

    EXPECT_FALSE(function.no_calls());
    EXPECT_EQ(function.calls(), 2U);
}

/// Test that the set_return(...) with multiple return values works as expected
TEST(call, set_multiple_return)
{
    stub::call<uint32_t(uint32_t)> function;
    function.set_return({5U, 10U, 15U});

    EXPECT_EQ(function(1U), 5U);
    EXPECT_EQ(function(2U), 10U);
    EXPECT_EQ(function(3U), 15U);

    // It repeats
    EXPECT_EQ(function(1U), 5U);
    EXPECT_EQ(function(2U), 10U);
    EXPECT_EQ(function(3U), 15U);
}

/// Test that the set_return(...) with const references work
TEST(call, set_return_const_reference)
{
    stub::call<const uint32_t&()> function;
    function.set_return({5U, 10U});

    const auto& v = function();

    EXPECT_EQ(v, 5U);
    EXPECT_EQ(function(), 10U);
}

/// Test that the number of calls work
TEST(call, calls_no_calls)
{
    stub::call<void()> function;
    EXPECT_TRUE(function.no_calls());
    function();
    EXPECT_FALSE(function.no_calls());
    EXPECT_EQ(function.calls(), 1U);

    function();
    EXPECT_FALSE(function.no_calls());
    EXPECT_EQ(function.calls(), 2U);
}

// Test that the call_arguments(...) function works as expected
TEST(call, call_arguments)
{
    stub::call<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);

    EXPECT_EQ(function.calls(), 2U);

    auto one = std::make_tuple(2,3);
    auto two = std::make_tuple(4,5);

    EXPECT_TRUE(one == function.call_arguments(0));
    EXPECT_TRUE(two == function.call_arguments(1));
}

// Test that the basic expect_calls() function works
TEST(call, expect_calls_with)
{
    stub::call<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);

    EXPECT_TRUE((bool)function.expect_calls()
                    .with(2,3)
                    .with(4,5));
}

// Test that the basic expect_calls() function works when we have more
// with statements than actual function calls and the other way
// around.
TEST(call, expect_calls_with_out_of_bounds)
{
    {
        stub::call<void(uint32_t,uint32_t)> function;

        EXPECT_FALSE((bool)function.expect_calls()
                        .with(2,3)
                        .with(4,5));
    }

    {
        stub::call<void(uint32_t,uint32_t)> function;

        EXPECT_FALSE((bool)function.expect_calls()
                         .with(2,3).repeat(10));
    }

    {
        stub::call<void(uint32_t,uint32_t)> function;

        function(2,3);
        function(4,5);

        EXPECT_FALSE((bool)function.expect_calls()
                         .with(2,3));
    }
}

// Test that we can "inject" the same expected calls with the repeat
// function
TEST(call, expect_calls_with_repeat)
{
    stub::call<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);
    function(4,5);
    function(4,5);
    function(2,3);

    EXPECT_TRUE((bool)function.expect_calls()
                    .with(2,3)
                    .with(4,5).repeat(2)
                    .with(2,3));

    EXPECT_TRUE((bool)function.expect_calls()
                    .with(2,3)
                    .with(4,5).repeat(1).repeat(1)
                    .with(2,3));

    EXPECT_FALSE((bool)function.expect_calls()
                    .with(2,2)
                    .with(4,5)
                    .with(4,5)
                    .with(4,5)
                    .with(2,3));
}

/// Test that the ignore function works as expected
TEST(call, expect_calls_with_ignore)
{
    stub::call<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);
    function(4,5);
    function(4,5);
    function(2,6);

    EXPECT_TRUE((bool)function.expect_calls()
                    .with(2,3)
                    .ignore(3)
                    .with(2,6));

    // EXPECT_TRUE((bool)function.expect_calls()
    //                 .any_calls_with(2,3));

    EXPECT_TRUE((bool)function.expect_calls()
                    .ignore(1)
                    .with(4,5).repeat(2)
                    .with(2,6));

    EXPECT_TRUE((bool)function.expect_calls()
                    .ignore(4)
                    .with(2,6));

    EXPECT_TRUE((bool)function.expect_calls()
                    .ignore(function.calls() - 1)
                    .with(2,6));
}

/// Test that we can use a binary predicate to provide custom
/// comparisons for types that do not support the default operator==
struct cup
{
    double m_volume;
};

TEST(call, expect_predicate_custom_type)
{
    stub::call<void(const cup&)> function;

    function(cup{2.3});
    function(cup{4.5});

    auto p = [](const std::tuple<cup>& a,
                const std::tuple<cup>& b) -> bool
        { return std::get<0>(a).m_volume == std::get<0>(b).m_volume; };

    EXPECT_TRUE((bool)function.expect_calls(p)
                .with(cup{2.3})
                .with(cup{4.5}));
}

// Test that a custom predicate that only checks for the second
// element of the pair works when checking for equality
TEST(call, expect_predicate_pair)
{
    using element = std::pair<uint32_t, uint32_t>;

    auto p = [](const std::tuple<element>& a,
                const std::tuple<element>& b) -> bool
        { return std::get<0>(a).second == std::get<0>(b).second; };

    stub::call<void(const element&)> function;
    function(element(2,3));

    // We only check for the second parameter so this should work
    EXPECT_TRUE((bool)function.expect_calls(p)
                    .with(element(10,3)));

    function(element(20,3));

    // We have called more than once
    EXPECT_FALSE((bool)function.expect_calls(p)
                    .with(element(10,3)));

    EXPECT_TRUE((bool)function.expect_calls(p)
                    .with(element(1,3))
                    .with(element(2,3)));

    // Try without the predicate
    EXPECT_FALSE((bool)function.expect_calls()
                    .with(element(1,3))
                    .with(element(2,3)));

    // The actual calls that were made
    EXPECT_TRUE((bool)function.expect_calls(p)
                    .with(element(2,3))
                    .with(element(20,3)));
}

// Test that with a custom predicate the first argument is the actual
// call value and the second argument is the expected. Because this is
// how we document it.
TEST(call, predicate_argument_order)
{
    auto p = [](const std::tuple<uint32_t>& actual,
                const std::tuple<uint32_t>& expected) -> bool
        {

            if (std::get<0>(actual) != 5U)
                return false;

            if (std::get<0>(expected) != 10U)
                return false;

            return true;
        };

    // Invoke the function with value 5 and expect value 10 should
    // work with our custom predicate function

    stub::call<void(uint32_t)> function;
    function(5);

    EXPECT_TRUE((bool)function.expect_calls(p)
                    .with(10U));
}
