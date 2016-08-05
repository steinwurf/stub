// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <tuple>
#include <functional>
#include <ostream>

/// @todo remove
#include <iostream>

#include "return_handler.hpp"
#include "unqualified_type.hpp"
#include "print_arguments.hpp"
#include "compare_call.hpp"

namespace stub
{
    /// Default function
    template<typename T> class function;

    /// @brief The function object act like a "sink" for function calls
    ///        i.e. we can define a function object to accept any type of
    ///        function call and it will simply store the arguments
    ///        for later inspection.
    ///
    /// The typical use-case for the function object is when testing that
    /// some code invokes a specific set of functions with a specific
    /// set of arguments.
    ///
    /// Example:
    ///
    ///    stub::function<void(uint32_t)> some_function;
    ///
    /// The above function takes an uint32_t and returns nothing, lets
    /// invoke it:
    ///
    ///     some_function(3);
    ///     some_function(4);
    ///
    /// Now we may check how the function was called:
    ///
    ///     bool called_once = some_function.expect_calls().with(3U);
    ///     assert(called_once == false);
    ///
    ///     bool called_with = some_function.expect_calls().with(4U);
    ///     assert(called_with == true);
    ///
    /// We can also define a function which returns a value:
    ///
    ///     stub::function<bool(uint32_t)> another_function;
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
    class function<R (Args...)>
    {
    public:

        /// Get the unqualified type, i.e. if the function takes a
        /// reference to some type we make sure we store the actual type.
        ///
        /// Example:
        ///
        ///     stub::function<void(const int&)> function;
        ///     function(3);
        ///
        /// In this case we will store an int of value 3 instead of a
        /// const& to the temporary value which will no longer exist
        /// when we want to compare.
        ///
        /// A tuple is used to store the arguments passed
        /// using arguments = std::tuple<typename unqualified_type<Args>::type...>;

    public:

        /// Represent a expectation of how the function object has been
        /// invoked. Using the API it is possible to setup how we
        /// expect the function object looks like. The expectation
        /// converts to bool allowing the user to detect whether the
        /// expectation was correct.
        struct expectation
        {
            /// @param the_function The function we configuring an expectation for
            ///
            /// @param predicate The function object used to compare the
            ///        call arguments
            expectation(const function& the_function)
                : m_function(the_function)
            { }

            /// Calling with(...) will add a set of arguments we
            /// expect to see. with(...) can be called multiple times
            /// in a row if we expect multiple function calls to the
            /// function object.
            ///
            /// As an example:
            ///
            ///     stub::function<void(uint32_t,uint32_t)> function;
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
            template<class... WithArgs>
            expectation& with(WithArgs&&... args)
            {
                //arguments<WithArgs...> a(std::forward<WithArgs>(args)...);
                //m_calls.emplace_back(std::move(a));
                return *this;
            }

            /// Convert the expectation to a boolean value either true
            /// of false depending on whether the expectations match
            /// the actual call.
            ///
            /// The first argument passed to the predicate function is
            /// the actual call arguments and the second is the
            /// expectation.
            ///
            /// @return True if the expectation matches the call,
            ///         otherwise false
            bool to_bool() const
            {
                // An expectation can't be evaluated if it hasn't been setup.
                assert(!m_calls.empty());
/*
                if (m_function.m_calls.size() != m_calls.size())
                    return false;

                for (uint32_t i = 0; i < m_calls.size(); ++i)
                {
                    auto& actual = m_function.m_calls[i];
                    if (m_calls[i].compare(actual) == false)
                    {
                        return false;
                    }
                }
*/
                return true;
            }

            /// Use the to_bool member function when casting thís expectation
            /// to a boolean value.
            ///
            /// @return True if the expectation matches the function,
            ///         otherwise false
            explicit operator bool() const
            {
                return to_bool();
            }

        private:

            /// The function we will check the expectation against
            const function& m_function;

            /// The expected calls
            std::vector<compare_call<Args...>> m_calls;
        };

    public:
        /// The call operator to "simulate" performing a function call.
        ///
        /// @param args The arguments that should be stored
        ///
        /// @return The return value generated by the return_handler
        R operator()(Args&&... args) const
        {
            m_calls.emplace_back(std::forward<Args>(args)...);
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
        return_handler<R>& set_return(const std::initializer_list<T> &returns)
        {
            return m_return_handler.set_return(returns);
        }

        /// @return The number of times the call operator has been invoked
        uint32_t calls() const
        {
            return (uint32_t)m_calls.size();
        }

        /// @return True if no calls have been made otherwise false
        bool no_calls() const
        {
            return m_calls.size() == 0;
        }

        /// @return The arguments passed to the n'th call
        const arguments<Args...>& call_arguments(uint32_t index) const
        {
            assert(index < m_calls.size());
            return m_calls[index];
        }

        /// Used when we want to check whether the function object is in a
        /// certain state. See examples usage in the expectation
        /// struct member functions.
        ///
        /// Two version of this function exists one which takes a
        /// binary predicate comparison function an one which does
        /// not. The version taking a predicate allows the user to
        /// provide a custom comparison function whereas the other
        /// version will rely on operator==(...).
        ///
        /// When you pass your own predicate function it will get the
        /// actual call arguments as the first parameter and the
        /// expected as the second.
        ///
        /// @return An expectation object
        expectation expect_calls() const
        {
            return expectation(*this);
        }

        /// Prints the status of the function object to the std::ostream.
        ///
        /// Example (using the output operator):
        ///
        ///    stub::function<void(uint32_t)> my_func;
        ///
        ///    my_func(4U);
        ///    my_func(5U);
        ///
        ///    // Print the current status of the function object,
        ///    std::cout << my_func << std::endl;
        ///
        /// @param out The ostream where the stub::function status should be
        void print(std::ostream& out) const
        {
            out << "Number of calls: " << m_calls.size() << std::endl;

            if(sizeof...(Args) == 0)
                return;

            for(uint32_t i = 0; i < m_calls.size(); ++i)
            {
                out << "Call " << i << ":\n";
                print_arguments(out, m_calls[i]);
            }
        }

    private:

        /// The return_handler manages the return values generated
        return_handler<R> m_return_handler;

        /// Stores the arguments every time the operator() is invoked
        mutable std::vector<arguments<Args...>> m_calls;
    };

    /// Output operator for printing function objects, see more info in
    /// stub::function::print(std::ostream&).
    ///
    ///
    /// @param out The output stream where the state of the function object
    ///        will be printed.
    ///
    /// @param function The function object we want to print
    ///
    /// @return The ostream operator.
    template<class T>
    inline std::ostream& operator<<(std::ostream& out, const function<T>& function)
    {
        function.print(out);
        return out;
    }

}
