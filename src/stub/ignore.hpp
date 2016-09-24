// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace stub
{
/// An object used to ignore values of specific arguments when comparing two
/// function calls.
///
/// Example:
///
///    stub::function<void(uint32_t, bool)> foo;
///
///    foo(3U, true);
///
///    assert(fool.expect_calls()
///        .with(3U, stub::ignore()));
///
struct ignore { };
}
