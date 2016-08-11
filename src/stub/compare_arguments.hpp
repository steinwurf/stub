// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>

#include "compare_argument.hpp"

namespace stub
{
    /// Specialization chosen for empty tuples or when Index reaches the
    /// sizeof the tuple (i.e. the number of values in the tuple), see
    /// description below.
    ///
    /// @return When the two tuples are empty they compare equal so we return
    ///         true
    template
    <
        class Index = std::integral_constant<uint32_t, 0U>,
        class... Args,
        class... WithArgs,
        class LastIndex = std::integral_constant<uint32_t, sizeof...(Args)>,
        typename std::enable_if<
            std::is_same<Index,LastIndex>::value, uint8_t>::type = 0
    >
    inline bool compare_arguments(const std::tuple<Args...>& actual,
                                  const std::tuple<WithArgs...>& with)
    {
        (void) actual;
        (void) with;

        return true;
    }

    /// Compare the content of two tuples.
    ///
    /// The two functions compare_arguments use SFINAE (Substitution Failure
    /// Is Not An Error) to select which overload to call.
    ///
    /// The overloading works like this:
    ///
    ///   1. If compare_arguments is called with an empty tuple then the
    ///      empty overload will be chosen.
    ///
    ///   2. If compare_arguments is called with a non-empty tuple the
    ///      Index!=LastIndex is true and the overload calling
    ///      compare_arugment(...) will be called. This will then recursively
    ///      call compare_arguments incrementing the Index. A some point
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
        class... WithArgs,
        class LastIndex = std::integral_constant<uint32_t, sizeof...(Args)>,
        typename std::enable_if<
            !std::is_same<Index,LastIndex>::value, uint8_t>::type = 0
    >
    inline bool compare_arguments(const std::tuple<Args...>& actual,
                                  const std::tuple<WithArgs...>& with)
    {
        static_assert(sizeof...(Args) == sizeof...(WithArgs),
            "The tuples must have same size");

        bool result = compare_argument(std::get<Index::value>(actual),
            std::get<Index::value>(with));

        if (result == false)
        {
            return result;
        }
        else
        {
            using next = std::integral_constant<uint32_t, Index::value + 1>;
            return compare_arguments<next>(actual, with);
        }
    }
}
