// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

namespace stub
{
    /// Returns a string representing the Steinwurf version of this library.
    inline std::string version()
    {
        #ifdef STEINWURF_STUB_VERSION
        return STEINWURF_STUB_VERSION;
        #else
        return "unknown";
        #endif
    }
}
