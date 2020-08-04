// Copyright (c) 2020 Stella Authors. All rights reserved.

#include <gtest/gtest.h>

#include "dll.h"

TEST(GenericMapTest, DllRetrieve)
{
    dy_t dy = get_sample_generic_map();

    ASSERT_EQ(dy_get_type(dy), dy_type_map);
    ASSERT_EQ(dy_get_map_len(dy), 3);

    dy_iter_t iter = dy_make_map_iter(dy);

    for (dy_keyval_t keyval = dy_get_map_iter(dy, iter); keyval.key != NULL;
         keyval             = dy_get_map_iter(dy, iter))
    {
        dy_t val = keyval.val;
        if (strcmp(keyval.key, "foo") == 0)
        {
            ASSERT_EQ(dy_get_type(val), dy_type_iarr);
            ASSERT_EQ(dy_get_iarr_len(val), 5);

            int64_t cmp[] = { 2, 5, 4, 8, 1 };

            for (int i = 0; i < 5; ++i)
                ASSERT_EQ(dy_get_iarr_idx(val, i), cmp[i]);
        }
        else if (strcmp(keyval.key, "bar") == 0)
        {
            ASSERT_EQ(dy_get_type(val), dy_type_str);
            ASSERT_EQ(dy_get_str_len(val), 5);
            ASSERT_STREQ(dy_get_str_data(val), "hello");
        }
        else
        {
            ASSERT_STREQ(keyval.key, "baz");
            ASSERT_EQ(dy_get_type(val), dy_type_i);
            ASSERT_EQ(dy_get_i(val), 15);
        }
    }

    dy_dispose_map_iter(iter);
    dy_dispose(dy);
}