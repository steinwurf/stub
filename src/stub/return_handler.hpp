// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <deque>

#include "unqualified_type.hpp"

namespace stub
{
/// @brief The return_handler is a helper object that is used
///        e.g. in the function object to control which return values
///        should be generated when called.
///
/// The return_handler provides the call operator() and when
/// invoked it will return the specified return value. The type R
/// will be the return value.
///
/// Example:
///
/// .. code-block:: c++
///    :linenos:
///
///        return_handler<uint32_t> v;
///        v.set_return(4U);
///
///        uint32_t a = v();
///        assert(a == 4U);
///
///        uint32_t b = v();
///        assert(b == 4U);
///
///        uint32_t c = v();
///        assert(c != 3U);
///        assert(c == 4U);
///
///
/// Or alternatively set multiple return values:
///
/// .. code-block:: c++
///    :linenos:
///
///        return_handler<uint32_t> v;
///        v.set_return({4U,3U});
///
///        uint32_t a = v();
///        assert(a == 4U);
///
///        uint32_t b = v();
///        assert(b == 3U);
///
///        uint32_t c = v();
///        assert(c == 4U);
///
///        uint32_t d = v();
///        assert(d != 4U);
///        assert(d == 3U);
///
///
/// The default behavior is to repeat the specified return values i.e.:
///
/// .. code-block:: c++
///    :linenos:
///
///        return_handler<uint32_t> v;
///        v.set_return(3U);
///
///        uint32_t a = v();
///        uint32_t b = v();
///        uint32_t c = v();
///
///        assert(a == 3U && b == 3U && c == 3U);
///
/// This behavior can be change by calling no_repeat() in which
/// case the return_handler can only be invoked once per return
/// value specified:
///
/// .. code-block:: c++
///    :linenos:
///
///        return_handler<uint32_t> v;
///        v.set_return(1U).no_repeat();
///
///        uint32_t a = v();
///        uint32_t b = v(); // <---- Crash
///
///        return_handler<uint32_t> v;
///        v.set_return({1U,2U,3U}).no_repeat();
///
///        uint32_t a = v();
///        uint32_t b = v();
///        uint32_t c = v();
///        uint32_t d = v(); // <---- Crash
///
///
template <class R>
class return_handler
{
public:
    /// Get the unqualified version of return type.
    using return_type = typename unqualified_type<R>::type;

    /// Constructor
    return_handler() : m_repeat(true), m_position(0)
    {
    }

    /// @todo remove this code. or consider a different way to handle this.
    /// Make the return_handler non-copyable
    // return_handler(const return_handler&) = delete;
    // return_handler& operator=(const return_handler&) = delete;

    /// Initializes the return_handler with the return values to
    /// use. Calling this function will also reset the
    /// return_handler state. So any previously specified returns
    /// values will be removed etc.
    ///
    /// @param values The list of return values to use
    ///
    /// @return Reference to the return handler, this allows the
    /// caller to perform additional customization to the return
    /// handler such as turn on or off repeat.
    template <class... Args>
    return_handler& set_return(Args&&... values)
    {
        m_repeat = true;
        m_position = 0;
        m_returns.clear();

        add_return(std::forward<Args>(values)...);

        return *this;
    }

    /// Set repeat off. This means that no values will be repeated
    /// the user has to specify exactly the number of values that
    /// should be return otherwise an assert will be triggered.
    /// @todo consider making opposite behavior default.
    void no_repeat()
    {
        m_repeat = false;
    }

    /// The call operator which will generate a return value.
    /// @return The generated return value
    R operator()() const
    {
        // Did you forget to add a return value?
        assert(m_returns.size() > 0);

        // If we are repeating and we have reached the end of the
        // list of return values
        if (m_repeat && (m_position == m_returns.size()))
            m_position = 0;

        assert(m_position < m_returns.size());

        R value = m_returns.at(m_position);
        ++m_position;

        return value;
    }

private:
    /// Overload that adds a return value
    void add_return(const return_type& value)
    {
        m_returns.push_back(value);
    }

    /// Add a number of return values
    template <class... Args>
    void add_return(const return_type& value, Args&&... more)
    {
        m_returns.push_back(value);
        add_return(std::forward<Args>(more)...);
    }

private:
    /// Boolean value controlling whether we should repeat return
    /// values when reaching the end of the return value vector or
    /// assert. True means we repeat, false means we should assert.
    bool m_repeat;

    /// The position of the return values vector that we will
    /// return upon next invocation of the call operator. The
    /// m_positions is mutable since the call operator is a const
    /// function and we need to increment m_positions once called.
    mutable uint32_t m_position;

    /// Container storing the return values to be used.
    /// We use a deque instead of a vector to avoid a specific issue with
    /// vector<bool>. vector<bool> is a bitset-like container, not a container
    /// of bools, which can cause unexpected behavior when used with certain STL
    /// algorithms. deque<bool> does not have this special behavior and is safer
    /// to use in this context.
    std::deque<return_type> m_returns;
};

/// Specialization for the case of a void function i.e. no return
/// value. We expect no calls to this return_handler the call
/// operator is only there to allow the code to compile when
/// e.g. the function class instantiates a return handler.
template <>
class return_handler<void>
{
public:
    /// Empty call operator
    void operator()() const
    {
    }
};
} // namespace stub
