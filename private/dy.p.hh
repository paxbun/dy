// Copyright (c) 2020 Chanjung Kim (paxbun). All rights reserved.

#include <dy.h>

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

typedef union _dy_data_t
{
  public:
    std::nullptr_t                        null;
    bool                                  b;
    int64_t                               i;
    double                                f;
    std::string                           str;
    std::vector<bool>                     barr;
    std::vector<uint8_t>                  bytes;
    std::vector<int64_t>                  iarr;
    std::vector<double>                   farr;
    std::vector<dy_t>                     arr;
    std::unordered_map<std::string, dy_t> map;
    ~_dy_data_t() {}
} dy_data_t;

struct _dy_val_t
{
  public:
    /// <summary>
    /// The type of the value
    /// </summary>
    dy_type_t type;

    /// <summary>
    /// The data of the value
    /// </summary>
    dy_data_t data;

    ~_dy_val_t() DY_NOEXCEPT
    {
        switch (type)
        {
        default:
        case dy_type_null:
        case dy_type_b:
        case dy_type_i:
        case dy_type_f: break;
        case dy_type_str: data.str.~basic_string(); break;
        case dy_type_bytes: data.bytes.~vector(); break;
        case dy_type_barr: data.barr.~vector(); break;
        case dy_type_iarr: data.iarr.~vector(); break;
        case dy_type_farr: data.farr.~vector(); break;
        case dy_type_arr: data.arr.~vector(); break;
        case dy_type_map: data.map.~unordered_map(); break;
        }
    }
};

struct _dy_iter_t
{
    std::unordered_map<std::string, dy_t>::iterator it;
};