// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <stub/return_handler.hpp>

TEST(return_handler, void)
{
    // Just checking that it can be instantiated
    stub::return_handler<void> r;
    r();
}

TEST(return_handler, api)
{
    // Return a single value multiple times
    {
        stub::return_handler<uint32_t> r;
        r.set_return(5U);

        EXPECT_EQ(r(), 5U);
        EXPECT_EQ(r(), 5U);
        EXPECT_EQ(r(), 5U);
    }

    // Use the return value of set_return
    {
        stub::return_handler<uint32_t> r;
        r.set_return({5U, 3U});

        EXPECT_EQ(r(), 5U);
        EXPECT_EQ(r(), 3U);
        EXPECT_EQ(r(), 5U);
        EXPECT_EQ(r(), 3U);
        EXPECT_EQ(r(), 5U);
    }

    // Try with no_repeat
    {
        stub::return_handler<uint32_t> r;
        r.set_return(5U).no_repeat();

        EXPECT_EQ(r(), 5U);

        r.set_return({3U,4U}).no_repeat();

        EXPECT_EQ(r(), 3U);
        EXPECT_EQ(r(), 4U);
    }

    // Death tests
    {
        stub::return_handler<uint32_t> r;
        r.set_return({3U,4U}).no_repeat();

        EXPECT_EQ(r(), 3U);
        EXPECT_EQ(r(), 4U);

        // death
        // EXPECT_EQ(r(), 4U);
    }
}
