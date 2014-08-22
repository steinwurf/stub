// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

namespace stub
{
    /// Get the unqualified type, i.e.
    ///
    ///    using v = typename unqualified_type<const uint32_t&>::type;
    ///
    /// Then v is uint32_t
    ///
    /// More info here:
    /// http://stackoverflow.com/questions/17295169
    /// http://flamingdangerzone.com/cxx11/2013/02/25/even-more-traits.html
    ///
    template<typename T>
    using unqualified_type =
        std::remove_cv<
        typename std::remove_reference<T>::type>;
}
