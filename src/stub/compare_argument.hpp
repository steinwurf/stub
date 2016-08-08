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
*/

    template<class Compare>
    struct compare
    {

        template<class Value>
        bool operator()(Value v)
        {
            return m_compare(v);
        }

        Compare m_compare;
    };

    template<class C>
    compare<C> make_compare(C c)
    {
        return compare<C>{ c };
    }


    template<class T, class Compare>
    inline bool compare_argument(T a, Compare t)
    {
        return t(a);
    }


}
