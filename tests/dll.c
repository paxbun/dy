// Copyright (c) 2020 Chanjung Kim (paxbun). All rights reserved.

#include "dll.h"

#include <assert.h>

DLL_PUBLIC(dy_t) get_sample_generic_array() DY_NOEXCEPT
{
    return dy_make_arr(
        (dy_t[]) { dy_make_str("hello"), dy_make_i(15), dy_make_b(true) }, 3);
}

DLL_PUBLIC(dy_t) get_sample_generic_map() DY_NOEXCEPT
{
    return dy_make_map(
        (dy_keyval_t[]) {
            { "foo", dy_make_iarr((int64_t[]) { 2, 5, 4, 8, 1 }, 5) },
            { "bar", dy_make_str("hello") },
            { "baz", dy_make_i(15) },
        },
        3);
}

DLL_PUBLIC(dy_t) get_sample_barr() DY_NOEXCEPT
{
    return dy_make_barr((bool[]) { true, false, true, true, false }, 5);
}

DLL_PUBLIC(dy_t) get_sample_bytes() DY_NOEXCEPT
{
    return dy_make_bytes((uint8_t[]) { 2, 3, 4, 1 }, 4);
}

DLL_PUBLIC(dy_t) get_sample_iarr() DY_NOEXCEPT
{
    return dy_make_iarr((int64_t[]) { 2, 3, 4, 1 }, 4);
}

DLL_PUBLIC(dy_t) get_sample_farr() DY_NOEXCEPT
{
    return dy_make_farr((double[]) { 2.5, 3.6, 3.8, 1.2, 4.5, 5.8 }, 6);
}