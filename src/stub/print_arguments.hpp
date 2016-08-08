// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "print_argument.hpp"

#include <tuple>
#include <type_traits>

namespace stub
{
    /// Specialization chosen for empty tuples or when Index reaches the
    /// sizeof the tuple (i.e. the number of values in the tuple), see
    /// description below.
    template
    <
        class Index = std::integral_constant<uint32_t, 0U>,
        class... Args,
        class LastIndex = std::integral_constant<uint32_t, sizeof...(Args)>,
        typename std::enable_if<
            std::is_same<Index,LastIndex>::value, uint8_t>::type = 0
    >
    inline void print_arguments(std::ostream& out, const std::tuple<Args...>& t)
    {
        (void) out;
        (void) t;
    }

    /// Prints the content of a tuple to the specified std::ostream.
    ///
    /// The two functions print_arguments use SFINAE (Substitution Failure
    /// Is Not An Error) to select which overload to call.
    ///
    /// The overloading works like this:
    ///
    ///   1. If print_arguments is called with an empty tuple then the
    ///      empty overload will be chosen.
    ///
    ///   2. If print_argument is called with a non-empty tuple the
    ///      Index!=LastIndex is true and the overload writing to the
    ///      std::ostream will be called. This will then recursively call
    ///      print_arguments incrementing the Index. A some point
    ///      Index==LastIndex and the empty overload gets chosen and we
    ///      are done.
    ///
    /// Note that initially this was implemented using a simpler technique
    /// similar to here: http://stackoverflow.com/a/6894436 but this did
    /// not work on with Microsoft Visual Studio 2013 so it was implemented
    /// using the std::integral_constant technique instead.
    template
    <
        class Index = std::integral_constant<uint32_t, 0U>,
        class... Args,
        class LastIndex = std::integral_constant<uint32_t, sizeof...(Args)>,
        typename std::enable_if<
            !std::is_same<Index,LastIndex>::value, uint8_t>::type = 0
    >
    inline void print_arguments(std::ostream& out, const std::tuple<Args...>& t)
    {
        print_argument(out, Index::value, std::get<Index::value>(t));

        print_arguments<
            std::integral_constant<uint32_t, Index::value + 1>>(out, t);
    }
}
