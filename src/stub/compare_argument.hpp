// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <type_traits>

#include "ignore.hpp"
#include "not_nullptr.hpp"
#include "compare.hpp"
#include "make_compare.hpp"

namespace stub
{
/// Compares two arguments of same type
template<class T, class U>
inline bool compare_argument(T a, U b)
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

/// Compare function where the second argument is not_nullptr - this evaluates
/// to true if the first argument is anything but nullptr
template<class T>
inline bool compare_argument(T a, not_nullptr)
{
    (void) a;
    static_assert(
        std::is_pointer<T>::value || std::is_same<T, std::nullptr_t>::value,
        "not_nullptr can only be used if T is a pointer type");
    return a != nullptr;
}

/// Compare argument using custom comparison functor.
template<class T, class Compare>
inline bool compare_argument(T a, compare<Compare> t)
{
    return t(a);
}
}
