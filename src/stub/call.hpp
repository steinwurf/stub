// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "return_handler.hpp"

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
    /// some code invokes a specific set of functions with a specific
    /// set of arguments.
    ///
    /// Example:
    ///
    ///    call<void(uint32_t)> some_function;
    ///
    /// The above call takes an uint32_t and returns nothing, lets
    /// invoke it:
    ///
    ///     some_function(3);
    ///     some_function(4);
    ///
    /// Now we may check how the function was called:
    ///
    ///     bool called_once = some_function.called_once_with(3U);
    ///     assert(called_once == false);
    ///
    ///     bool called_with = some_function.called_with(4U);
    ///     assert(called_with == true);
    ///
    /// We can also define a call which returns a value:
    ///
    ///     call<bool(uint32_t)> another_function;
    ///
    /// Here we have to specify what return value we expect:
    ///
    ///     another_function.set_return(true);
    ///
    ///     bool a = another_function(23);
    ///     bool b = another_function(13);
    ///
    ///     assert(a == true);
    ///     assert(b == true);
    ///
    /// For more information on the options for return values see the
    /// return_handler.hpp
    ///
    template<typename R, typename... Args>
    class call<R (Args...)>
    {

    public:
        /// A tuple is used to store the arguments passed
        typedef std::tuple<Args...> arguments;

    public:
        /// The call operator to "simulate" performing a function call.
        ///
        /// @param args The arguments that should be stored
        ///
        /// @return The return value generated by the return_handler
        R operator()(Args... args) const
        {
            m_calls.emplace_back(args...);
            return m_return_handler();
        }

        /// @copydoc return_handler::set_return(const T&)
        template<class T>
        return_handler<R>& set_return(const T& return_value)
        {
            return m_return_handler.set_return(return_value);
        }

        /// @copydoc return_handler::set_return(
        ///              const std::initializer_list<T>&)
        template<class T>
        return_handler<R> set_return(const std::initializer_list<T> &returns)
        {
            return m_return_handler.set_return(returns);
        }

        /// Checks if only one function call has been performed with
        /// the specified tuple of arguments. Will compare the two argument
        /// tuples with bool operator==(const arguments&, const arguments&)
        ///
        /// @param args List of arguments to check with most recent
        /// invocation of the call
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
        /// the specified tuple of arguments. The provided BinaryPredicate
        /// allows a custom comparison function.
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
        /// the specified arguments. The provided BinaryPredicate
        /// allows a custom comparison function.
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

        /// The has_calls function is used to check for a specify set
        /// of function calls. It will only return true if the number
        /// of calls plus the call arguments match. Also arguments
        /// should be given in the same order as they were received by
        /// the call operator(). Will compare the argument
        /// tuples with bool operator==(const arguments&, const arguments&)
        ///
        /// @param calls Vector of arguments to check with the
        /// invocations of the call
        ///
        /// @return True if the calls matches all recorded calls
        /// otherwise false.
        bool has_calls(const std::vector<arguments> &calls)
        {
            auto p = [](const arguments& a, const arguments& b) -> bool
                { return a == b; };

            return has_calls(calls, p);
        }

        /// The has_calls function is used to check for a specify set
        /// of function calls. It will only return true if the number
        /// of calls plus the call arguments match. Also arguments
        /// should be given in the same order as they were received by
        /// the call operator(). The provided BinaryPredicate
        /// allows a custom comparison function.
        ///
        /// @param calls Vector of arguments to check with the
        /// invocations of the call
        ///
        /// @param predicate Function object that provides bool
        /// operator()(const arguments&, const arguments&) to compare
        /// two argument tuples
        ///
        /// @return True if the calls matches all recorded calls
        /// otherwise false.
        template<class BinaryPredicate>
        bool has_calls(const std::vector<arguments> &calls,
                       const BinaryPredicate& predicate)
        {
            if (m_calls.size() != calls.size())
                return false;

            for(uint32_t i = 0; i < m_calls.size(); ++i)
            {
                if (!predicate(m_calls[i], calls[i]))
                {
                    return false;
                }
            }

            return true;
        }

        /// @return The number of times the call operator has been invoked
        uint32_t calls() const
        {
            return m_calls.size();
        }

    private:

        /// The return_handler manages the return values generated
        return_handler<R> m_return_handler;

        /// Stores the arguments every time the operator() is invoked
        mutable std::vector<arguments> m_calls;
    };
}
