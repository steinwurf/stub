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
    /// @todo fix
    /// http://stackoverflow.com/questions/17295169
    /// http://flamingdangerzone.com/cxx11/2013/02/25/even-more-traits.html

    template<typename T>
    using unqualified_type =
        typename std::remove_cv<
        typename std::remove_reference<T>::type>::type;
}
