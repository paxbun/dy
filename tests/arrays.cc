// Copyright (c) 2020 Chanjung Kim (paxbun). All rights reserved.

#include <gtest/gtest.h>

#include "dll.h"
#include <vector>

using namespace std;

TEST(ArrayTest, BooleanArray)
{
    dy_t arr = get_sample_generic_barr();
    ASSERT_EQ(dy_get_type(arr), dy_type_barr);

    vector<int64_t> cmp { true, false, true, true, false };
    ASSERT_EQ(cmp.size(), dy_get_barr_len(arr));

    for (int i = 0; i < 5; ++i) ASSERT_EQ(cmp[i], dy_get_barr_idx(arr, i));

    dy_dispose(arr);
}

TEST(ArrayTest, IntegerArray)
{
    dy_t arr = get_sample_generic_iarr();
    ASSERT_EQ(dy_get_type(arr), dy_type_iarr);

    vector<int64_t> cmp { 2, 3, 4, 1 };

    int64_t const*  data = dy_get_iarr_data(arr);
    vector<int64_t> vec(data, data + dy_get_iarr_len(arr));

    ASSERT_EQ(vec, cmp);

    dy_dispose(arr);
}

TEST(ArrayTest, FloatArray)
{
    dy_t arr = get_sample_generic_farr();
    ASSERT_EQ(dy_get_type(arr), dy_type_farr);

    vector<double> cmp { 2.5, 3.6, 3.8, 1.2, 4.5, 5.8 };

    double const*  data = dy_get_farr_data(arr);
    vector<double> vec(data, data + dy_get_farr_len(arr));

    for (int i = 0; i < 6; ++i) ASSERT_DOUBLE_EQ(cmp[i], vec[i]);

    dy_dispose(arr);
}