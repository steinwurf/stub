// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "compare.hpp"

namespace stub
{
    /// Small helper function for creating compare objects.
    ///
    /// Example:
    ///
    ///    // Build a comparison function from a lambda
    ///    auto compare = stub::make_compare([](uint32_t v)->bool
    ///        { return v == 1U}; );
    ///
    ///    assert(compare(1U) == true);
    ///    assert(compare(2U) == false);
    template<class C>
    compare<C> make_compare(C c)
    {
        return compare<C> { c };
    }

}
