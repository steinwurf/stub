// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <ostream>
#include <iomanip>
#include <cstdint>

namespace stub
{
/// Default printer - just use the std::ostream operator<< to output
/// the values to the stream.
template<class T>
inline void print_argument(std::ostream& out, uint32_t index, T value)
{
    out << "Arg " << index << ": " << value << "\n";
}

/// Overload of the default printer function for pointer types.
///
/// The reason we overload for pointers is that we cannot rely on the
/// default std::ostream operator<< for printing pointers. std::ostream
/// operator<< is overloaded for e.g. char* (and on most platforms also
/// uint8_t*) to try to print it as a zero-terminated string.
///
/// However, in many applications dealing with binary data i.e. uint8_t
/// pointers are not to strings but to actual binary data. In these
/// cases trying to print the data as a zero-terminated string leads to
/// all sorts of out-of-bounds memory access.
template<class T>
inline void print_argument(std::ostream& out, uint32_t index, T* value)
{
    // The easy approach i.e. just using the std::ostream overload for
    // void* works, but the string representation is not the same on
    // Linux, OSX and Windows so unit-tests will fail. Instead we have
    // to work a bit more to print the value of the pointer as hex but
    // using the code below we get the same string representation on
    // all tested platforms.
    out << "Arg " << std::dec << std::noshowbase << index << ": "
        << std::hex << std::showbase << (uintptr_t)value << "\n";
}
}
