// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <tuple>
#include <functional>

#include "return_handler.hpp"
#include "unqualified_type.hpp"

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

        /// Get the unqualified type, i.e. if the function takes a
        /// reference to some type we make sure we store the actual type.
        ///
        /// Example:
        ///
        ///     stub::call<void(const int&)> function;
        ///     function(3);
        ///
        /// In this case we will store an int of value 3 instead of a
        /// const reference to the temporary value which will no long
        /// exist when we want to compare.
        ///
        /// A tuple is used to store the arguments passed
        typedef std::tuple<unqualified_type<Args>...> arguments;

        /// The default binary predicate type use when comparing
        /// function calls
        using default_predicate =
            std::function<bool(const arguments&,const arguments&)>;

    public:

        /// Represent a expectation of how the call object has been
        /// invoked. Using the API it is possible to setup how we
        /// expect the call object to look like. The expectation
        /// convert to bool allowing the user to detect whether the
        /// expecatation was correct.
        template<class BinaryPredicate>
        struct expectation
        {
            /// @param the_call The call we configuring an expectation for
            /// @param predicate The function object used to compare the
            ///        call arguments
            expectation(const call& the_call, const BinaryPredicate& predicate)
                : m_call(the_call),
                  m_predicate(predicate)
            { }

            /// Calling with(...) will add a set of arguments we
            /// expect to see. with(...) can be called multiple times
            /// in a row if we expect multiple function calls to the
            /// call object.
            ///
            /// As an example:
            ///
            ///     stub::call<void(uint32_t,uint32_t)> function;
            ///     function(3,1);
            ///     function(4,2);
            ///
            ///     assert(function.expect_calls()
            ///                .with(3,1).with(4,2));
            ///
            ///
            /// @param args The arguments for a function call
            ///
            /// @return The expectation itself, which allows chaining
            ///         function calls
            expectation& with(Args... args)
            {
                m_calls.emplace_back(args...);
                return *this;
            }

            /// Calling repeat(...) will copy the arguments of the
            /// last with(...) a number of times. The idea is that if
            /// you expect to see a number of identical function calls
            /// you can avoid adding them one-by-one using with.
            ///
            /// As an example:
            ///
            ///     stub::call<void()> function;
            ///     function();
            ///     function();
            ///     function();
            ///     function();
            ///
            ///     assert(function.expect_calls()
            ///                .with().repeat(3));
            ///
            /// Is the same as:
            ///
            ///     stub::call<void()> function;
            ///     function();
            ///     function();
            ///     function();
            ///     function();
            ///
            ///     assert(function.expect_calls()
            ///                .with()
            ///                .with()
            ///                .with()
            ///                .with());
            ///
            /// @param times The number of times we repeat the last
            ///        with(...) call
            ///
            /// @return The expectation itself, which allows chaining
            ///         function calls
            expectation& repeat(uint32_t times)
            {
                assert(times >= 1);
                assert(m_calls.size() > 0);

                for (uint32_t i = 0; i < times; ++i)
                {
                    m_calls.push_back(m_calls.back());
                }

                return *this;
            }

            /// Calling ignore(...) will make us disregard a number of
            /// actual function calls. This means that when we evaluate
            /// or expectation we will ignore the values passed in the
            /// ignored calls.
            ///
            /// As an example:
            ///
            ///     stub::call<void(uint32_t,uint32_t)> function;
            ///     function(3,1);
            ///     function(4,2);
            ///     function(5,0);
            ///
            ///     assert(function.expect_calls()
            ///                .ignore(2).with(5,0));
            ///
            /// Here we ignore the first two calls and only check the
            /// last one. The ignore can be used in between with(...)
            /// calls if wanted.
            ///
            /// As an example:
            ///
            ///     stub::call<void(uint32_t,uint32_t)> function;
            ///     function(3,1);
            ///     function(4,2);
            ///     function(5,0);
            ///
            ///     assert(function.expect_calls()
            ///                with(3,1).ignore(1).with(5,0));
            ///
            /// Here we ignore the arguments to the second call and
            /// check only the first and last calls.
            ///
            /// @param times The number of calls to ignore
            ///
            /// @return The expectation itself, which allows chaining
            ///         function calls
            expectation& ignore(uint32_t calls)
            {
                assert(calls > 0);

                // We skip the arguments already added
                uint32_t skip = m_calls.size();

                // Now simply copy the ignored calls from the "real" call
                // which means they will always match
                for (uint32_t i = 0; i < calls; ++i)
                {
                    uint32_t index = skip + i;
                    m_calls.push_back(m_call.m_calls[index]);
                }

                return *this;
            }

            /// Convert the expectation to a boolean value either true
            /// of false depending on whether the expectations match
            /// the actual call.
            ///
            /// @return True if the expectation matches the call,
            ///         otherwise false
            explicit operator bool() const
            {
                return std::equal(std::begin(m_call.m_calls),
                                  std::end(m_call.m_calls),
                                  std::begin(m_calls),
                                  m_predicate);
            }

        private:

            /// The call we will check the expectation against
            const call& m_call;

            /// The comparison function used when comparing whether
            /// the function call arguments match
            BinaryPredicate m_predicate;

            /// The expected calls
            std::vector<arguments> m_calls;
        };

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

        /// @return The number of times the call operator has been invoked
        uint32_t calls() const
        {
            return m_calls.size();
        }

        /// @return True if no calls have been made otherwise false
        bool no_calls() const
        {
            return m_calls.size() == 0;
        }

        /// @return The arguments passed to the n'th call
        const arguments& call_arguments(uint32_t index) const
        {
            assert(index < m_calls.size());
            return m_calls[index];
        }

        /// Used when we want to check whether the call object is in a
        /// certain state. See examples usage in the expectation
        /// struct member functions.
        ///
        /// Two version of this function exists one which takes a
        /// binary predicate comparison function an one which does
        /// not. The version taking a predicate allows the user to
        /// provide a custom comparison function whereas the other
        /// version will rely on operator==(...).
        ///
        /// @return An expectation object
        expectation<default_predicate> expect_calls()
        {
            default_predicate predicate =
                [](const arguments& a, const arguments& b) -> bool
                   { return a == b; };

            return expectation<default_predicate>(*this, predicate);
        }

        /// @copydoc expect_calls()
        template<class BinaryPredicate>
        expectation<BinaryPredicate> expect_calls(
            const BinaryPredicate& predicate)
        {
            return expectation<BinaryPredicate>(*this, predicate);
        }

    private:

        /// The return_handler manages the return values generated
        return_handler<R> m_return_handler;

        /// Stores the arguments every time the operator() is invoked
        mutable std::vector<arguments> m_calls;
    };
}
