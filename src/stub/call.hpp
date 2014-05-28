// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

namespace stub
{
    /// Default call
    template<typename T> class call;

    /// @brief The call object act like a "sink" for function calls
    /// i.e. we can define a call object to accept any type of
    /// function call and it will simply store the arguments for later
    /// inspection.
    ///
    /// The typical use-case for the call object is when testing that
    /// some codes invokes a specfic set of functions with a specific
    /// set of arguments.
    ///
    /// Example:
    ///
    ///    call<void(uint32_t)> some_function;
    ///
    /// The above call takes a uint32_t and returns nothing, lets
    /// invoke it:
    ///
    /// some_function(3);
    /// some_function(4);
    ///
    /// Now we may check how the function was called:
    ///
    /// bool called_once = some_function.called_once_with(3U);
    /// assert(called_once == false);
    ///
    /// bool called_with = some_function.called_with(4U);
    /// assert(called_with == true);
    ///
    /// We can also define a call which returns a value:
    ///
    ///     call<bool(uint32_t)> another_function;
    ///
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
