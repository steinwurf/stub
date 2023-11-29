// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

namespace stub
{

/// Exception thrown when a call is not expected
struct expect_calls : public std::exception
{
    /// Constructor
    expect_calls(const std::string& message) : m_message(message)
    {
    }

    /// Destructor
    const char* what() const throw()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

}
