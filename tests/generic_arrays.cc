// Copyright (c) 2020 Chanjung Kim (paxbun). All rights reserved.

#include <gtest/gtest.h>

#include "dll.h"

dy_t get_doubled_generic_array(dy_t arr) DY_NOEXCEPT
{
    assert(dy_get_type(arr) == dy_type_arr);

    size_t      len  = dy_get_arr_len(arr);
    dy_t*       narr = (dy_t*)malloc(sizeof(dy_t) * len * 2);
    const dy_t* data = dy_get_arr_data(arr);

    for (size_t i = 0; i < len; ++i) narr[i] = dy_copy(data[i]);
    for (size_t i = 0; i < len; ++i) narr[i + len] = dy_copy(data[i]);

    dy_dispose(arr);

    dy_t rtn = dy_make_arr(narr, len * 2);
    free(narr);

    return rtn;
}

TEST(GenericArrayTest, DllRetrieve)
{
    dy_t arr = get_sample_generic_array();

    ASSERT_EQ(dy_get_type(arr), dy_type_arr);
    ASSERT_EQ(dy_get_arr_len(arr), 3);

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 0)), dy_type_str);
    ASSERT_STREQ(dy_get_str_data(dy_get_arr_idx(arr, 0)), "hello");

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 1)), dy_type_i);
    ASSERT_EQ(dy_get_i(dy_get_arr_idx(arr, 1)), 15);

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 2)), dy_type_b);
    ASSERT_EQ(dy_get_b(dy_get_arr_idx(arr, 2)), true);

    dy_dispose(arr);
}

TEST(GenericArrayTest, DoubledArray)
{
    dy_t arr = get_sample_generic_array();
    arr      = get_doubled_generic_array(arr);

    ASSERT_EQ(dy_get_type(arr), dy_type_arr);
    ASSERT_EQ(dy_get_arr_len(arr), 6);

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 0)), dy_type_str);
    ASSERT_STREQ(dy_get_str_data(dy_get_arr_idx(arr, 0)), "hello");
    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 3)), dy_type_str);
    ASSERT_STREQ(dy_get_str_data(dy_get_arr_idx(arr, 3)), "hello");
    ASSERT_NE(dy_get_arr_idx(arr, 0), dy_get_arr_idx(arr, 3));

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 1)), dy_type_i);
    ASSERT_EQ(dy_get_i(dy_get_arr_idx(arr, 1)), 15);
    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 4)), dy_type_i);
    ASSERT_EQ(dy_get_i(dy_get_arr_idx(arr, 4)), 15);
    ASSERT_NE(dy_get_arr_idx(arr, 1), dy_get_arr_idx(arr, 4));

    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 2)), dy_type_b);
    ASSERT_EQ(dy_get_b(dy_get_arr_idx(arr, 2)), true);
    ASSERT_EQ(dy_get_type(dy_get_arr_idx(arr, 5)), dy_type_b);
    ASSERT_EQ(dy_get_b(dy_get_arr_idx(arr, 5)), true);
    ASSERT_NE(dy_get_arr_idx(arr, 2), dy_get_arr_idx(arr, 5));

    dy_dispose(arr);
}