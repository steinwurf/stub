// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <stub/call.hpp>

TEST(call, api)
{
    {
        stub::call<void()> function;
        function();
        EXPECT_TRUE(function.called_once_with());
        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::call<void(uint32_t)> function;
        function(2U);
        EXPECT_TRUE(function.called_once_with(2U));
        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::call<uint32_t(uint32_t)> function;
        function.set_return(5U);

        EXPECT_TRUE(function.no_calls());

        auto a = function(2U);

        EXPECT_FALSE(function.no_calls());

        EXPECT_EQ(a, 5U);
        EXPECT_TRUE(function.called_once_with(2U));
        EXPECT_EQ(function.calls(), 1U);

        auto b = function(8U);
        EXPECT_EQ(b, 5U);
        EXPECT_TRUE(function.called_with(8U));
        EXPECT_EQ(function.calls(), 2U);

        EXPECT_TRUE(function.has_calls({
                        std::make_tuple(2U),
                        std::make_tuple(8U)}));
    }
}

TEST(call, predicate)
{
    // Custom predicate that only checks for the second element of the
    // pair when checking for equality
    {
        using element = std::pair<uint32_t, uint32_t>;

        auto p = [](const std::tuple<element>& a,
                    const std::tuple<element>& b) -> bool
            { return std::get<0>(a).second == std::get<0>(b).second; };

        stub::call<void(element)> function;
        function(element(2,3));

        // We only check for the second parameter so this should work
        EXPECT_TRUE(function.called_once_with(element(10,3), p));

        function(element(20,3));

        // We have called more than once
        EXPECT_FALSE(function.called_once_with(element(10,3), p));

        std::vector<std::tuple<element>> calls =
            {
                std::make_tuple(element(1,3)),
                std::make_tuple(element(2,3))
            };

        EXPECT_TRUE(function.has_calls(calls, p));

        // Try without the predicate
        EXPECT_FALSE(function.has_calls(calls));

        // The actual calls that were made
        std::vector<std::tuple<element>> real_calls =
            {
                std::make_tuple(element(2,3)),
                std::make_tuple(element(20,3))
            };

        // Try without the predicate
        EXPECT_TRUE(function.has_calls(real_calls));
    }

    {
        stub::call<void(uint32_t)> function;
        function(2U);
        EXPECT_TRUE(function.called_once_with(2U));
        EXPECT_EQ(function.calls(), 1U);

        function(2U);
        EXPECT_FALSE(function.called_once_with(2U));
        EXPECT_EQ(function.calls(), 2U);

    }

    {
        stub::call<uint32_t(uint32_t)> function;
        function.set_return(5U);
        auto a = function(2U);

        EXPECT_EQ(a, 5U);
        EXPECT_TRUE(function.called_once_with(2U));
        EXPECT_EQ(function.calls(), 1U);

        auto b = function(8U);
        EXPECT_EQ(b, 5U);
        EXPECT_TRUE(function.called_with(8U));
        EXPECT_EQ(function.calls(), 2U);

        EXPECT_TRUE(function.has_calls({
                        std::make_tuple(2U),
                        std::make_tuple(8U)}));
    }
}


TEST(call, arguments)
{
    stub::call<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);

    EXPECT_EQ(function.calls(), 2);

    auto one = std::make_tuple(2,3);
    auto two = std::make_tuple(4,5);

    EXPECT_TRUE(one == function.call_arguments(0));
    EXPECT_TRUE(two == function.call_arguments(1));
}
