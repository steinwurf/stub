// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/function.hpp>

#include <gtest/gtest.h>

/// Test that the call operator works as expected
TEST(test_function, call_operator)
{
    {
        stub::function<void()> function;
        function();

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::function<void(uint32_t)> function;
        function(2U);

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::function<void(const uint32_t&)> function;
        function(2U);

        EXPECT_EQ(function.calls(), 1U);
    }

    {
        stub::function<void(uint32_t&)> function;

        {
            uint32_t i = 2U;
            function(i);
        }

        EXPECT_EQ(function.calls(), 1U);

        uint32_t j = 2U;
        EXPECT_TRUE(function.expect_calls().with(j).to_bool());
    }
}

/// Test that the set_return(...) function works as expected
TEST(test_function, clear)
{
    stub::function<uint32_t(uint32_t)> function;
    function.set_return(5U);

    EXPECT_EQ(function.calls(), 0U);

    auto a = function(2U);

    EXPECT_EQ(a, 5U);
    EXPECT_EQ(function.calls(), 1U);

    function.clear();
    function.set_return(8U);

    EXPECT_EQ(function.calls(), 0U);

    auto b = function(8U);

    EXPECT_EQ(b, 8U);

    EXPECT_EQ(function.calls(), 1U);
}

/// Test that the set_return(...) function works as expected
TEST(test_function, set_return)
{
    stub::function<uint32_t(uint32_t)> function;
    function.set_return(5U);

    EXPECT_EQ(function.calls(), 0U);

    auto a = function(2U);

    EXPECT_EQ(a, 5U);

    EXPECT_TRUE(function.calls() != 0U);
    EXPECT_EQ(function.calls(), 1U);

    auto b = function(8U);

    EXPECT_EQ(b, 5U);

    EXPECT_FALSE(function.calls() == 0);
    EXPECT_EQ(function.calls(), 2U);
}

/// Test that the set_return(...) with multiple return values works as expected
TEST(test_function, set_multiple_return)
{
    stub::function<uint32_t(uint32_t)> function;
    function.set_return(5U, 10U, 15U);

    EXPECT_EQ(function(1U), 5U);
    EXPECT_EQ(function(2U), 10U);
    EXPECT_EQ(function(3U), 15U);

    // It repeats
    EXPECT_EQ(function(1U), 5U);
    EXPECT_EQ(function(2U), 10U);
    EXPECT_EQ(function(3U), 15U);
}

/// Test that the set_return(...) with const references work
TEST(test_function, set_return_const_reference)
{
    stub::function<const uint32_t& ()> function;
    function.set_return(5U, 10U);

    const auto& v = function();

    EXPECT_EQ(v, 5U);
    EXPECT_EQ(function(), 10U);
}

/// Test that the number of calls work
TEST(test_function, calls_no_calls)
{
    stub::function<void()> function;
    EXPECT_TRUE(function.calls() == 0U);
    function();
    EXPECT_FALSE(function.calls() == 0U);
    EXPECT_EQ(function.calls(), 1U);

    function();
    EXPECT_FALSE(function.calls() == 0U);
    EXPECT_EQ(function.calls(), 2U);
}

// Test that the call_arguments(...) function works as expected
TEST(test_function, call_arguments)
{
    stub::function<void(uint32_t,uint32_t)> function;

    function(2,3);
    function(4,5);

    EXPECT_EQ(function.calls(), 2U);

    auto one = std::make_tuple(2,3);
    auto two = std::make_tuple(4,5);

    EXPECT_TRUE(one == function.call_arguments(0));
    EXPECT_TRUE(two == function.call_arguments(1));
}

// Test that the basic expect_calls() function works
TEST(test_function, expect_calls_with)
{
    stub::function<void(uint8_t,uint8_t)> function8;

    function8(2U, 3U);
    function8(4U, 5U);

    EXPECT_TRUE(function8.expect_calls()
                .with(2U, 3U)
                .with(4U, 5U)
                .to_bool());

    stub::function<void(uint32_t,uint32_t)> function32;

    function32(2U, 3U);
    function32(4U, 5U);

    EXPECT_TRUE(function32.expect_calls()
                .with(2U, 3U)
                .with(4U, 5U)
                .to_bool());

    stub::function<void(std::string)> function_string;

    function_string("hello");
    function_string("world");

    EXPECT_TRUE(function_string.expect_calls()
                .with("hello")
                /// Force the compare_call class to store an std::string. The call above
                /// will be stored as a char pointer, and hence have no heap allocations
                /// needing clean up.
                .with(std::string("world"))
                .to_bool());
}

// Test that the basic expect_calls() function works when we have more
// with statements than actual function calls and the other way
// around.
TEST(test_function, expect_calls_with_out_of_bounds)
{
    {
        stub::function<void(uint32_t,uint32_t)> function;

        EXPECT_FALSE(function.expect_calls()
                     .with(2U, 3U)
                     .with(4U, 5U)
                     .to_bool());
    }

    {
        stub::function<void(uint32_t,uint32_t)> function;

        function(2U, 3U);
        function(4U, 5U);

        EXPECT_FALSE(function.expect_calls()
                     .with(2U, 3U)
                     .to_bool());
    }
}

/// Test that the check function works as expected
TEST(test_function, use_to_bool)
{
    stub::function<void(int)> function;

    function(1);
    function(2);

    EXPECT_TRUE(function.expect_calls()
                .with(1)
                .with(2)
                .to_bool());

    EXPECT_FALSE(function.expect_calls()
                 .with(1)
                 .to_bool());
}

/// Test that we can use a binary predicate to provide custom
/// comparisons for types that do not support the default operator==
struct cup
{
    double m_volume;
};

TEST(test_function, expect_predicate_custom_type)
{
    /// @todo implement
    stub::function<void(const cup&)> function;

    function(cup{2.3});
    function(cup{4.5});

    auto compare = [](double expected, const cup& c)-> bool
    { return c.m_volume == expected; };

    EXPECT_TRUE(function.expect_calls()
                .with(stub::make_compare(
                    std::bind(compare, 2.3, std::placeholders::_1)))
                .with(stub::make_compare(
                    std::bind(compare, 4.5, std::placeholders::_1)))
                .to_bool());
}

// Test that a custom predicate that only checks for the second
// element of the pair works when checking for equality
TEST(test_function, expect_predicate_pair)
{
    using element = std::pair<uint32_t, uint32_t>;

    auto expect = [](uint32_t expected, const element& actual) -> bool
    { return expected == actual.second; };

    auto expect_3 = stub::make_compare(
        std::bind(expect, 3, std::placeholders::_1));

    stub::function<void(const element&)> function;
    function(element(2,3));

    // We only check for the second parameter so this should work
    EXPECT_TRUE(function.expect_calls()
                .with(expect_3)
                .to_bool());

    function(element(20,3));

    // We have called more than once
    EXPECT_FALSE(function.expect_calls()
                 .with(expect_3)
                 .to_bool());

    EXPECT_TRUE(function.expect_calls()
                .with(expect_3)
                .with(expect_3)
                .to_bool());

    // Try without the predicate
    EXPECT_FALSE(function.expect_calls()
                 .with(element(1,3))
                 .with(element(2,3))
                 .to_bool());

    // The actual calls that were made
    EXPECT_TRUE(function.expect_calls()
                .with(element(2,3))
                .with(element(20,3))
                .to_bool());

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

// Test that with a custom predicate the first argument is the actual
// call value and the second argument is the expected. Because this is
// how we document it.
TEST(test_function, predicate_argument_order)
{
    auto compare = [](uint32_t value) -> bool
    {
        if (value != 5U)
            return false;

        return true;
    };

    // Invoke the function with value 5 and expect value 10 should
    // work with our custom predicate function

    stub::function<void(uint32_t)> function;
    function(5);

    EXPECT_TRUE(function.expect_calls()
                .with(stub::make_compare(compare))
                .to_bool());
}

// Test that we can pretty-print the function object, where the function takes
// no arguments
TEST(test_function, pretty_print_without_arguments)
{
    stub::function<void()> function;

    function();
    function();

    std::stringstream stream;
    stream << function;

    EXPECT_EQ(stream.str(), "Number of calls: 2\n");
}

// Test that we can pretty-print the function object, where the function takes
// arguments
TEST(test_function, pretty_print_with_arguments)
{
    stub::function<void(uint32_t, uint32_t)> function;

    function(2,3);
    function(4,5);

    std::stringstream stream;
    stream << function;

    EXPECT_EQ(stream.str(), "Number of calls: 2\n"
              "Call 0:\n"
              "Arg 0: 2\n"
              "Arg 1: 3\n"
              "Call 1:\n"
              "Arg 0: 4\n"
              "Arg 1: 5\n");
}

// Test that the expect_calls().with(...) igore arguments function works
TEST(test_function, expect_calls_with_ignore)
{
    stub::function<void(uint32_t,uint32_t)> function;

    function(2U, 3U);
    function(4U, 5U);

    EXPECT_TRUE(function.expect_calls()
                .with(2U, stub::ignore())
                .with(stub::ignore(), 5U)
                .to_bool());
}

TEST(test_function, value_by_reference)
{
    stub::function<void(uint32_t)> function;

    uint32_t i = 3U;
    function(i);

    EXPECT_TRUE(function.expect_calls().with(3U).to_bool());
}

namespace
{
    struct you_shall_not_copy
    {
        you_shall_not_copy() = default;
        you_shall_not_copy(const you_shall_not_copy& other) = delete;

        you_shall_not_copy(const you_shall_not_copy&& other)
        {
            (void) other;
            // do move
        }

        uint32_t m_value = 0;
    };
}

// Test that non-copyable types work with stub
TEST(test_function, non_copyable_type)
{
    you_shall_not_copy yc;

    stub::function<void(you_shall_not_copy)> function;

    function(std::move(yc));
    function(you_shall_not_copy());

    EXPECT_EQ(function.calls(), 2U);
}

/// This test was added due to a memory leak.
/// It ensures that objects stored stored by the compare_call class have their
/// destructor invoked.
namespace
{
struct leak_counter
{
    /// implement copy constructor
    leak_counter(const leak_counter& other) :
        m_count(other.m_count)
    {
        ++m_count;
    }

    /// Delete copy assignment constructor
    leak_counter& operator=(const leak_counter&) = delete;

    leak_counter(uint8_t& count) :
        m_count(count)
    {
        ++m_count;
    }

    ~leak_counter()
    {
        --m_count;
    }

    uint8_t& m_count;
};

inline bool operator==(const leak_counter&, const leak_counter&)
{
    return true;
}
}

TEST(test_function, check_virtual_destructor)
{
    uint8_t count = 0U;
    {
        leak_counter c(count);
        stub::function<void(leak_counter&)> test_function;
        EXPECT_FALSE(test_function.expect_calls()
                     .with(c)
                     .to_bool());
    }

    EXPECT_EQ(0U, count);
}
