// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <stub/return_handler.hpp>

#include <gtest/gtest.h>

#include <initializer_list>

TEST(test_return_handler, void)
{
    // Just checking that it can be instantiated
    stub::return_handler<void> r;
    r();
}

TEST(test_return_handler, api)
{
    // Return a single value multiple times
    {
        stub::return_handler<uint32_t> r;
        r.set_return(5U);

        EXPECT_EQ(5U, r());
        EXPECT_EQ(5U, r());
        EXPECT_EQ(5U, r());
    }

    // Use the return value of set_return
    {
        stub::return_handler<uint32_t> r;
        r.set_return(5U, 3U);

        EXPECT_EQ(5U, r());
        EXPECT_EQ(3U, r());
        EXPECT_EQ(5U, r());
        EXPECT_EQ(3U, r());
        EXPECT_EQ(5U, r());
    }

    // Try with no_repeat
    {
        stub::return_handler<uint32_t> r;
        r.set_return(5U).no_repeat();

        EXPECT_EQ(5U, r());

        r.set_return(3U,4U).no_repeat();

        EXPECT_EQ(3U, r());
        EXPECT_EQ(4U, r());
    }

    // Death tests
    {
        stub::return_handler<uint32_t> r;
        r.set_return(3U,4U).no_repeat();

        EXPECT_EQ(3U, r());
        EXPECT_EQ(4U, r());

        // death
        // EXPECT_EQ(r(), 4U);
    }

    // String
    {
        stub::return_handler<const char*> r;
        r.set_return("3U","4U").no_repeat();

        EXPECT_EQ("3U", r());
        EXPECT_EQ("4U", r());
    }

    // String
    {
        std::initializer_list<const char*> a {"3U","4U"};

        stub::return_handler<const char*> r;
        r.set_return(a).no_repeat();

        EXPECT_EQ("3U", r());
        EXPECT_EQ("4U", r());
    }
}
