// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "ignore.hpp"
#include "compare.hpp"
#include "make_compare.hpp"

namespace stub
{
    /// Compares two arguments of same type
    template<class T>
    inline bool compare_argument(T a, T b)
    {
        return a == b;
    }

    /// Compare function where the second argument is ignore - this always
    /// compares true
    template<class T>
    inline bool compare_argument(T a, ignore)
    {
        (void) a;
        return true;
    }

    /// Compare argument using custom comparison functor.
    template<class T, class Compare>
    inline bool compare_argument(T a, Compare t)
    {
        return t(a);
    }
}
