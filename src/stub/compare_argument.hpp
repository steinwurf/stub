// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace stub
{
    /// Compares two arguments
    template<class T>
    inline bool compare_argument(T a, T b)
    {
        return a == b;
    }
/*
    template<class T>
    inline bool compare_argument(T a, ignore)
    {
        (void)a;
        return true;
    }

    template<class T>
    inline bool compare_argument(T a, compare<T> t)
    {
        return t.m_compare(a);
    }
*/

}
