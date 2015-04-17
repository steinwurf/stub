// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <ostream>
#include <cstdint>
#include <tuple>
#include <type_traits>


namespace stub
{
    inline void print_arguments(std::ostream& out, const std::tuple<>& t)
    {
        (void) out;
        (void) t;
    }

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
    ///   1. If print_arguments is called with an empty tuple then
    ///      Index==sizeof...(Args) will be true and the empty overload
    ///      will be chosen.
    ///
    ///   2. If print_argument is called with a non-empty tuple the
    ///      Index!=sizeof(Args) is true and the overload writing to the
    ///      std::ostream will be called. This will then recursively call
    ///      print_arguments incrementing the Index. A some point
    ///      Index==sizeof(Args) and the empty overload gets chosen and we
    ///      are done.
    ///
    ///Note that if called with an empty tuple then
    /// Index != sizeof...(Args) will be false and the empty
    /// print_arguments will be called
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
        out << "Arg " << Index::value << ": "
            << std::get<Index::value>(t) << "\n";

        print_arguments<
            std::integral_constant<uint32_t, Index::value + 1>>(out, t);
    }
}
