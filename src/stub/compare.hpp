// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace stub
{
/// An object used to customize comparison of specific arguments when
/// comparing two function calls.
///
/// Example:
///
///    bool test(uint32_t v)
///    {
///        return v == 1U;
///    }
///
///    stub::compare<bool(*)(uint32_t)> compare = { test };
///
///    assert(compare(1U) == true);
///    assert(compare(2U) == false);
///
template<class Compare>
struct compare
{
    /// Call operator which calls the compare function with the passed value
    template<class Value>
    bool operator()(Value v)
    {
        return m_compare(v);
    }

    /// The comparison function
    Compare m_compare;
};
}
