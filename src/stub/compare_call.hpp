// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <memory>

#include "arguments.hpp"
#include "compare_arguments.hpp"

namespace stub
{
/// This stores a tuple of types that is to, at some point, be compared with
/// a different tuple of arguments.
///
/// The basic usage is as follows:
///
/// .. code-block:: c++
///    :linenos:
///
///        auto expectation = std::make_tuple(5, true, 4.0);
///        compare_call<int, bool, double> expect(expectation);
///        auto actual = std::make_tuple(4, true, 4.0);
///
///
/// Now compare our expectation with the actual values:
///
/// .. code-block:: c++
///    :linenos:
///
///        bool result = expect.compare(actual);
///        assert(result == false);
///
///
/// The compare_call::compare(...) function uses compare_arguments(...)
/// to compare the elements of the two tuples element-wise using
/// compare_argument(...). In the case above the two tuples contain the same
/// types. So every thing is pretty straight-forward.
///
/// However, in some cases we want to customize the comparison of two
/// element. This could be that we simply don't care about a specific value
/// or we want to use a user-define comparison function.
///
/// To support this use-case we allow special values to be used in the
/// comparison tuple. The two tuples can therefore have different types,
/// making the implementation a bit more involved and the reason for the
/// type-erausre implemented in this class.
///
/// Lets see how this would work in practice.
///
/// Notice how we can pass ignore() as the second argument to our
/// expectation tuple:
///
/// .. code-block:: c++
///    :linenos:
///
///        auto expectation = std::make_tuple(5, ignore(), 4.0);
///        compare_call<int, bool, double> expect(expectation);
///        auto actual = std::make_tuple(5, true, 4.0);
///
///
/// Now compare our expectation with the actual values:
///
/// .. code-block:: c++
///    :linenos:
///
///        bool result = expect.compare(actual);
///        assert(result == true);
///
///
/// The reason this works when comparing the two tuples element-wise is that
/// the compare_argument(...) function has an overload that accepts ignore
/// as the second argument. By specializing compare_argument(...) we can
/// extend support for more special values to support custom behaviour.
///
template <class... Args>
struct compare_call
{
    /// Construct a new call comparison
    template <class... WithArgs>
    compare_call(WithArgs&&... expected)
    {
        assert(!m_implementation);

        m_implementation =
            std::unique_ptr<interface>(new implementation<WithArgs...>(
                std::forward<WithArgs>(expected)...));
    }

    /// @return Compare the values of the passed tuple with those of the
    ///         expectation.
    bool compare(const arguments<Args...>& actual) const
    {
        assert(m_implementation);
        return m_implementation->compare(actual);
    }

private:
    /// Interface used in the type erasure
    struct interface
    {
        virtual bool compare(const arguments<Args...>& value) const = 0;
        virtual ~interface()
        {
        }
    };

    // Container for the expected values
    template <class... WithArgs>
    struct implementation : public interface
    {
        implementation(WithArgs&&... expected) :
            m_expected(std::forward<WithArgs>(expected)...)
        {
        }

        bool compare(const arguments<Args...>& actual) const override
        {
            return compare_arguments(actual, m_expected);
        }

        /// The tuple containing the expected values
        arguments<WithArgs...> m_expected;
    };

private:
    /// Stores the type-erased expectation
    std::unique_ptr<interface> m_implementation;
};
}
