// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <initializer_list>

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
///    return_handler<uint32_t> v;
///    v.set_return(4U);
///
///    uint32_t a = v();
///    assert(a == 4U);
///
///    uint32_t b = v();
///    assert(b == 4U);
///
///    uint32_t c = v();
///    assert(c != 3U);
///    assert(c == 4U);
///
/// Or alternatively set multiple return values:
///
///    return_handler<uint32_t> v;
///    v.set_return({4U,3U});
///
///    uint32_t a = v();
///    assert(a == 4U);
///
///    uint32_t b = v();
///    assert(b == 3U);
///
///    uint32_t c = v();
///    assert(c == 4U);
///
///    uint32_t d = v();
///    assert(d != 4U);
///    assert(d == 3U);
///
/// The default behavior is to repeat the specified return values i.e.:
///
///    return_handler<uint32_t> v;
///    v.set_return(3U);
///
///    uint32_t a = v();
///    uint32_t b = v();
///    uint32_t c = v();
///
///    assert(a == 3U && b == 3U && c == 3U);
///
/// This behavior can be change by calling no_repeat() in which
/// case the return_handler can only be invoked once per return
/// value specified:
///
///    return_handler<uint32_t> v;
///    v.set_return(1U).no_repeat();
///
///    uint32_t a = v();
///    uint32_t b = v(); // <---- Crash
///
///    return_handler<uint32_t> v;
///    v.set_return({1U,2U,3U}).no_repeat();
///
///    uint32_t a = v();
///    uint32_t b = v();
///    uint32_t c = v();
///    uint32_t d = v(); // <---- Crash
///
template<class R>
class return_handler
{
public:

    /// Get the unqualified version of return type.
    using return_type = typename unqualified_type<R>::type;

    /// Constructor
    return_handler() :
        m_repeat(true),
        m_position(0)
    { }

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
    template<class... Args>
    return_handler& set_return(Args&&... values)
    {
        m_repeat = true;
        m_position = 0;
        m_returns.clear();

        add_return(std::forward<Args>(values)...);

        return *this;
    }

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
    return_handler& set_return(std::initializer_list<return_type> values)
    {
        m_repeat = true;
        m_position = 0;
        m_returns.clear();

        for (auto& r : values)
            add_return(r);

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

        R value = m_returns[m_position];
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
    template<class... Args>
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

    /// Vector storing the return values to be used.
    std::vector<return_type> m_returns;
};

/// Specialization for the case of a void function i.e. no return
/// value. We expect no calls to this return_handler the call
/// operator is only there to allow the code to compile when
/// e.g. the function class instantiates a return handler.
template<>
class return_handler<void>
{
public:

    /// Empty call operator
    void operator()() const
    { }
};
}
