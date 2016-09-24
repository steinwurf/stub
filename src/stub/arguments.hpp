// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <tuple>

namespace stub
{
/// Tuple containing the values actually passed in the call
template<class... Args>
using arguments = decltype(std::make_tuple(std::declval<Args>()...));
}
