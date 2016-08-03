// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>
#include <cassert>

#include "compare_arguments.hpp"

namespace stub
{
    template<class... Args>
    struct expect_arguments
    {

        bool compare(const std::tuple<Args...>& values)
        {
            assert(m_values);
            return m_values->compare(values);
        }

        template<class... WithArgs>
        void with(const std::tuple<WithArgs...>& values)
        {
            assert(!m_values);
            m_values = std::unique_ptr<interface>(
                new implementation<WithArgs...>(values));
        }

    private:
        
        struct interface
        {
            virtual bool compare(const std::tuple<Args...>& values) = 0;
        };

        template<class... WithArgs>
        struct implementation : public interface
        {
            implementation(const std::tuple<WithArgs...>& values)
            {
                m_values = values;
            }

            bool compare(const std::tuple<Args...>& values) override
            {
                return compare_arguments(values, m_values);
            }

            std::tuple<WithArgs...> m_values;
        };

        std::unique_ptr<interface> m_values;
    };


}
