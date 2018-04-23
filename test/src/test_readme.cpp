// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
    template<class T>
    void function_to_test(T& t)
    {
        t.some_function(4);

        if (t.some_check())
        {
            t.do_something_else(false);
        }
    }

    struct dummy
    {
        stub::function<void(int)> some_function;
        stub::function<bool()> some_check;
        stub::function<void(bool)> do_something_else;
    };
}
/// Tests that all the examples given in the readme works as advertised
TEST(test_readme, usage)
{
        // Construct object
        dummy obj;
        obj.some_check.set_return(true);

        // Run function
        function_to_test(obj);

        // Check calls
        assert(obj.some_function.expect_calls().with(4));
        assert(obj.some_check.calls() == 1);
        assert(obj.do_something_else.expect_calls().with(false));
}

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
    assert(some_function.calls() != 0);
}

TEST(test_readme, clear_the_state_of_the_function_object)
{
    stub::function<uint32_t(uint32_t)> some_function;
    some_function.set_return(5);

    uint32_t a = some_function(3);
    uint32_t b = some_function(4);

    assert(a == 5);
    assert(b == 5);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    some_function.clear();

    // Return true if no calls were made
    assert(some_function.calls() == 0);

    // Before we can use some_function again we have to set a new return value
    some_function.set_return(6);
    uint32_t c = some_function(1);

    assert(c == 6);
}

TEST(test_readme, clear_only_the_function_calls_state)
{
    stub::function<uint32_t(uint32_t)> some_function;
    some_function.set_return(5);

    uint32_t a = some_function(3);
    uint32_t b = some_function(4);

    assert(a == 5);
    assert(b == 5);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    some_function.clear_calls();

    // Return true if no calls were made
    assert(some_function.calls() == 0);

    // We can continue to call the function
    uint32_t c = some_function(1);

    assert(c == 5);
}

TEST(test_readme, get_the_arguments_of_a_specific_call)
{
    stub::function<void(uint32_t,uint32_t)> function;

    function(3U,4U);
    function(4U,3U);
    function(2U,6U);

    auto a = function.call_arguments(1);
    auto b = std::make_tuple(4U,3U);

    assert(a == b);
}

TEST(test_readme, ignore_specific_arguments_of_a_function_call)
{
    stub::function<void(uint32_t,uint32_t)> function;

    function(3U,4U);
    function(4U,3U);

    // Is matched by:
    bool works = function.expect_calls()
                 .with(stub::ignore(), 4U)
                 .with(4U, stub::ignore())
                 .to_bool();

    assert(works);
}

TEST(test_readme, ensure_specific_arguments_of_a_function_call_not_nullptr)
{
    stub::function<void(uint8_t*, uint32_t)> function;

    std::vector<uint8_t> buffer(1);
    function(buffer.data(), buffer.size());

    // Is matched by:
    bool works = function.expect_calls()
                 .with(stub::not_nullptr(), 1U)
                 .to_bool();

    assert(works);
}
namespace
{
struct cup
{
    double m_volume;
};
}


TEST(test_readme, comparing_custom_arguments1)
{
    stub::function<void(const cup&)> function;

    function(cup{2.3});
    function(cup{4.5});

    assert(function.expect_calls()
           .with(stub::make_compare([](auto& c) {return c.m_volume == 2.3;}))
    .with(stub::make_compare([](auto& c) {return c.m_volume == 4.5;}))
    .to_bool());
}

TEST(test_readme, comparing_custom_arguments2)
{
    using element = std::pair<uint32_t, uint32_t>;

    auto expect = [](uint32_t expected, const element& actual) -> bool
    { return expected == actual.second; };

    stub::function<void(const element&)> function;
    function(element(2,3));
    function(element(20,3));

    using namespace std::placeholders;
    // We have called the function more than once
    assert(false == function.expect_calls()
           .with(stub::make_compare(std::bind(expect, 3, _1))).to_bool());

    // Works since we only match the second value of the pair
    assert(true == function.expect_calls()
           .with(stub::make_compare(std::bind(expect, 3, _1)))
           .with(stub::make_compare(std::bind(expect, 3, _1))).to_bool());

    // Without the custom comparison it fails
    assert(false == function.expect_calls()
           .with(element(1,3))
           .with(element(2,3)).to_bool());
}

TEST(test_readme, building_an_expectation)
{
    {
        stub::function<void(uint32_t)> some_function;

        // Call the function
        for (uint32_t i = 0; i < 10; ++i)
        {
            some_function(i);
        }

        // Check the expectation.
        assert(some_function.expect_calls()
               .with(0U)
               .with(1U)
               .with(2U)
               .with(3U)
               .with(4U)
               .with(5U)
               .with(6U)
               .with(7U)
               .with(8U)
               .with(9U));
    }

    {
        stub::function<void(uint32_t)> some_function;

        auto some_function_expectation = some_function.expect_calls();

        // Call the function and setup expectation
        for (uint32_t i = 0; i < 10; ++i)
        {
            some_function(i);
            some_function_expectation.with(i);
        }

        // Check the expectation.
        assert(some_function_expectation);
    }
}

TEST(test_readme, function_return_values)
{
    {
        stub::function<bool(uint32_t)> some_function;

        some_function.set_return(true);

        bool a = some_function(23U);
        bool b = some_function(13U);

        assert(a == true);
        assert(b == true);
    }

    {
        stub::function<uint32_t()> some_function;

        some_function.set_return(4U,3U);

        uint32_t a = some_function();
        assert(a == 4U);

        uint32_t b = some_function();
        assert(b == 3U);

        uint32_t c = some_function();
        assert(c == 4U);

        uint32_t d = some_function();
        assert(d != 4U);
        assert(d == 3U);
    }

    {
        stub::function<uint32_t()> some_function;
        some_function.set_return(3U);

        uint32_t a = some_function();
        uint32_t b = some_function();
        uint32_t c = some_function();

        assert(a == 3U && b == 3U && c == 3U);
    }


    {
        stub::function<uint32_t()> some_function;
        some_function.set_return(1U).no_repeat();

        uint32_t a = some_function();
        // uint32_t b = some_function(); // <---- Will crash

        some_function.set_return(1U,2U,3U).no_repeat();

        uint32_t e = some_function();
        uint32_t f = some_function();
        uint32_t g = some_function();
        // uint32_t h = some_function(); // <---- Will crash

        assert(a == 1U && e == 1U && f == 2U && g == 3U);
    }
}

namespace
{
struct paper
{
    // Call the print function on the printer object
    template<class Printer>
    void print(Printer& printer)
    {
        printer.print("Hello world");
    }
};

// Test stub printer object
struct printer
{
    stub::function<void(std::string)> print;
};
}

TEST(test_readme, using_stub_with_template_arguments)
{
    printer printer;
    paper hello;

    hello.print(printer);

    assert(printer.print.expect_calls()
           .with("Hello world")
           .to_bool());
}
namespace
{
struct static_paper
{
    // Call the static print function on the Printer type
    template<class Printer>
    void print()
    {
        Printer::print("Hello world");
    }
};

struct static_printer
{
    static stub::function<void(std::string)> print;
};

stub::function<void(std::string)> static_printer::print;
}
TEST(test_readme, using_static_stub_with_template_arguments)
{
    static_paper hello;

    hello.print<static_printer>();

    assert(static_printer::print.expect_calls()
           .with("Hello world")
           .to_bool());
}
