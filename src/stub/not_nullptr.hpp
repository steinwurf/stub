// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace stub
{
/// An object used to check if a value is not a null pointer,
/// when comparing two function calls.
///
/// Example:
///
///    stub::function<void(uint32_t, uint8_t*)> foo;
///
///    std::vector<uint8_t> buffer(10);
///    foo(3U, buffer.data());
///
///    assert(fool.expect_calls()
///        .with(3U, stub::not_nullptr()));
///
struct not_nullptr
{
};
}
