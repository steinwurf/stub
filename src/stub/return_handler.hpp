// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

namespace stub
{
    /// @brief The return_handler is a helper object that is used
    /// e.g. in the call object to control which return values should
    /// be generated when called.
    ///
    /// The return_handler provides the call operator() and when
    /// invoked it will return the specified return value. The type R
    /// will be the return value.
    ///
    /// Example:
    ///
    ///    return_handler<uint32_t> v;
    ///    v.set_return(4U);
    ///    v.set_return(3U);
    ///
    ///    uint32_t a = v();
    ///    assert(a == 4U);
    ///
    ///    uint32_t b = v();
    ///    assert(b == 3U);
    ///
    /// Or alternatively using the set_returns() function:
    ///
    ///    return_handler<uint32_t> v;
    ///    v.set_returns({4U,3U});
    ///
    ///    uint32_t a = v();
    ///    assert(a == 4U);
    ///
    ///    uint32_t b = v();
    ///    assert(b == 3U);
    ///
    template<class R>
    class return_handler
    {
    public:

        /// Constructor
        return_handler()
            : m_position(0)
        {}

        /// Add
        void set_return(const R& value)
        {
            m_returns.push_back(value);
        }

        void set_returns(const std::initializer_list<R>& values)
        {
            m_returns.insert(m_returns.end(),
                             values.begin(), values.end());
        }

        R operator()() const
        {
            assert(m_position < m_returns.size());

            R value = m_returns[m_position];
            ++m_position;

            return value;
        }

        mutable uint32_t m_position;
        std::vector<R> m_returns;
    };

    /// Specialization for
    template<>
    class return_handler<void>
    {
    public:

        void operator()() const
        {
        }
    };
}
