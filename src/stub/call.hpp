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

    /// Default call
    template<typename T> class call;

    /// Specialization accepting a function signature
    template<typename R, typename... Args> class call<R (Args...)>
    {
    public:

        /// The call operator to "simulate" performing
        R operator()(Args... args) const
        {
            m_calls.emplace_back(args...);
            return m_return_handler();
        }

        /// @copydoc return_handler::set_return(const T&)
        template<class T>
        void set_return(const T& return_value)
        {
            m_return_handler.set_return(return_value);
        }

        /// @copydoc return_handler::set_returns(
        ///              const std::initializer_list<T>&)
        template<class T>
        void set_returns(const std::initializer_list<T> &returns)
        {
            m_return_handler.set_returns(returns);
        }

        /// Checks if only one function call has been performed with
        /// the specified tuple of arguments. Providing a BinaryPredicate
        /// allows us to customize the comparison function.
        ///
        /// @param args List of arguments to check with most recent
        /// invocation of the call
        ///
        /// @param predicate Function object that provides bool
        /// operator()(const arguments&, const arguments&) to compare
        /// two argument tuples
        ///
        /// @return True if the args matches the most recent call
        /// otherwise false.
        bool called_once_with(Args... args) const
        {
            auto p = [](const arguments& a, const arguments& b) -> bool
                { return a == b; };

            return called_once_with(args..., p);
        }

        /// Checks if only one function call has been performed with
        /// the specified tuple of arguments. Providing a BinaryPredicate
        /// allows us to customize the comparison function.
        ///
        /// @param args List of arguments to check with most recent
        /// invocation of the call
        ///
        /// @param predicate Function object that provides bool
        /// operator()(const arguments&, const arguments&) to compare
        /// two argument tuples
        ///
        /// @return True if the args matches the most recent call
        /// otherwise false.
        template<class BinaryPredicate>
        bool called_once_with(
            Args... args, const BinaryPredicate& predicate) const
        {
            if (m_calls.size() != 1U)
            {
                return false;
            }

            return called_with(args..., predicate);
        }

        /// Checks is the most recent function call was performed with
        /// the specified arguments. Will compare the two argument
        /// tuples with bool operator==(const arguments&, const arguments&)
        ///
        /// @param args List of arguments to check with most recent
        /// invocation of the call
        ///
        /// @return True if the args matches the most recent call
        /// otherwise false.
        bool called_with(Args... args) const
        {
            auto p = [](const arguments& a, const arguments& b) -> bool
                { return a == b; };

            return called_with(args..., p);
        }

        /// Checks is the most recent function call was performed with
        /// the specified arguments. Providing a BinaryPredicate
        /// allows us to customize the comparison function.
        ///
        /// @param args List of arguments to check with most recent
        /// invocation of the call
        ///
        /// @param predicate Function object that provides bool
        /// operator()(const arguments&, const arguments&) to compare
        /// two argument tuples
        ///
        /// @return True if the args matches the most recent call
        /// otherwise false.
        template<class BinaryPredicate>
        bool called_with(Args... args, const BinaryPredicate& predicate) const
        {
            assert(m_calls.size() > 0);

            // We know there is one element in the list
            const auto& stored_args = m_calls.back();
            const auto& reference_args = std::make_tuple(args...);

            // Apply the binary predicate to the last stored call
            return predicate(stored_args, reference_args);
        }

        /// The return_handler manages the return values generated
        return_handler<R> m_return_handler;

        /// Stores the arguments every time the operator() is invoked
        mutable std::vector<arguments> m_calls;
    };
}
