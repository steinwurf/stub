// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

namespace stub
{

    template<class R>
    class return_handler
    {
    public:

        return_handler()
            : m_position(0)
        {}

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

    template<>
    class return_handler<void>
    {
    public:

        void operator()() const
        {
        }
    };
}
