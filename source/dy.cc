// Copyright (c) 2020 Stella Authors. All rights reserved.

#include <dy.p.hh>

#include <cassert>
#include <cstdlib>
#include <type_traits>
#include <utility>

using namespace std;

#define DY_TYPE(f) dy_type_##f

#define DY_ASSERT(t)                                                           \
    assert(val != nullptr);                                                    \
    assert(val->type == DY_TYPE(t));

#define DY_DECL(f) (declval<_dy_val_t>().data.f)

#define DY_DECLTYPE(f) remove_reference_t<decltype(DY_DECL(f))>

#define DY_DATA(f) (val->data.f)

#define DY_MAKE(f)                                                             \
    DY_PUBLIC(dy_t)                                                            \
    dy_make_##f(DY_DECLTYPE(f) data) DY_NOEXCEPT                               \
    {                                                                          \
        return new _dy_val_t {                                                 \
            .type = DY_TYPE(f),                                                \
            .data = { .f = data },                                             \
        };                                                                     \
    }

#define DY_GET(f)                                                              \
    DY_PUBLIC(DY_DECLTYPE(f)) dy_get_##f(dy_t val) DY_NOEXCEPT                 \
    {                                                                          \
        DY_ASSERT(f);                                                          \
        return DY_DATA(f);                                                     \
    }

#define DY_MAKE_LEN(f)                                                         \
    DY_PUBLIC(dy_t)                                                            \
    dy_make_##f(DY_DECLTYPE(f)::value_type const* ptr, size_t len) DY_NOEXCEPT \
    {                                                                          \
        assert(ptr != nullptr || len == 0);                                    \
        return new _dy_val_t {                                                 \
            .type = DY_TYPE(f),                                                \
            .data = { .f = make_data_len<DY_DECLTYPE(f)>(ptr, len) },          \
        };                                                                     \
    }

#define DY_GET_LEN(f)                                                          \
    DY_PUBLIC(size_t) dy_get_##f##_len(dy_t val) DY_NOEXCEPT                   \
    {                                                                          \
        DY_ASSERT(f);                                                          \
        return DY_DATA(f).size();                                              \
    }

#define DY_GET_DATA(f)                                                         \
    DY_PUBLIC(DY_DECLTYPE(f)::value_type const*)                               \
    dy_get_##f##_data(dy_t val) DY_NOEXCEPT                                    \
    {                                                                          \
        DY_ASSERT(f);                                                          \
        return DY_DATA(f).data();                                              \
    }

#define DY_GET_IDX(f)                                                          \
    DY_PUBLIC(DY_DECLTYPE(f)::value_type)                                      \
    dy_get_##f##_idx(dy_t val, size_t idx) DY_NOEXCEPT                         \
    {                                                                          \
        DY_ASSERT(f);                                                          \
        return DY_DATA(f)[idx];                                                \
    }

#define DY_COPY_HELPER(f)                                                      \
    case dy_type_##f:                                                          \
        return new _dy_val_t                                                   \
        {                                                                      \
            .type = dy_type_##f, .data = { .f = DY_DATA(f) },                  \
        }

namespace
{

/// <summary>
/// checks whether the type is valid. Used in assertions.
/// </summary>
/// <param name="type">the type to check</param>
/// <returns><c>true</c> if valid, <c>false</c> otherwise</returns>
inline bool valid_type(dy_type_t type) DY_NOEXCEPT
{
    return dy_type_null <= type && type <= dy_type_map;
}

template <typename T>
T default_value = 0;

template <>
bool default_value<bool> = false;

template <>
dy_t default_value<dy_t> = nullptr;

template <>
char default_value<char> = ' ';

/// <summary>
/// makes a container from the pointer
/// </summary>
/// <typeparam name="T">the type of the container</typeparam>
/// <param name="ptr">the pointer to the entries to copy</param>
/// <param name="len">the number of the entries</param>
/// <returns>a new container instance</returns>
template <typename T>
T make_data_len(typename T::value_type const* ptr, size_t len) DY_NOEXCEPT
{
    if (ptr != nullptr) return T(ptr, ptr + len);
    else
        return T(len, default_value<typename T::value_type>);
}

}

DY_PUBLIC(dy_type_t) dy_get_type(dy_t val) DY_NOEXCEPT
{
    assert(val != nullptr);
    assert(valid_type(val->type));
    return val->type;
}

DY_PUBLIC(dy_t) dy_copy(dy_t val) DY_NOEXCEPT
{
    assert(val != nullptr);
    assert(valid_type(val->type));

    switch (val->type)
    {
        DY_COPY_HELPER(null);
        DY_COPY_HELPER(b);
        DY_COPY_HELPER(i);
        DY_COPY_HELPER(f);
        DY_COPY_HELPER(str);
        DY_COPY_HELPER(bytes);
        DY_COPY_HELPER(barr);
        DY_COPY_HELPER(iarr);
        DY_COPY_HELPER(farr);
    case dy_type_arr:
    {
        // TODO: rewrite this with std::ranges::views::transform
        vector<dy_t> arr;
        arr.reserve(DY_DATA(arr).size());
        for (auto val : DY_DATA(arr)) arr.push_back(dy_copy(val));
        return new _dy_val_t {
            .type = dy_type_arr,
            .data = { .arr = arr },
        };
    }
    case dy_type_map:
    {
        // TODO: rewrite this with std::ranges::views::transform
        unordered_map<string, dy_t> map;
        map.reserve(DY_DATA(map).size());
        for (auto const& [str, dy] : DY_DATA(map))
            map.insert(make_pair(str, dy_copy(dy)));
        return new _dy_val_t {
            .type = dy_type_arr,
            .data = { .map = map },
        };
    }
    }

    return dy_make_null();
}

DY_PUBLIC(void) dy_dispose(dy_t val) DY_NOEXCEPT
{
    switch (val->type)
    {
    case dy_type_arr:
        for (auto dy : DY_DATA(arr)) dy_dispose(dy);
        break;
    case dy_type_map:
        for (auto const& [str, dy] : DY_DATA(map)) dy_dispose(dy);
        break;
    }

    dy_dispose_self(val);
}

DY_PUBLIC(void) dy_dispose_self(dy_t val) DY_NOEXCEPT
{
    delete val;
}

// ---------------------------------- null ---------------------------------- //

DY_PUBLIC(dy_t) dy_make_null() DY_NOEXCEPT
{
    return new _dy_val_t {
        .type = dy_type_null,
        .data = { .null = nullptr },
    };
}

// ----------------------------------- b  ----------------------------------- //

DY_MAKE(b) DY_GET(b);

// ----------------------------------- i  ----------------------------------- //

DY_MAKE(i) DY_GET(i);

// ----------------------------------- f  ----------------------------------- //

DY_MAKE(f) DY_GET(f);

// ---------------------------------- str  ---------------------------------- //

DY_PUBLIC(dy_t) dy_make_str(char const* str) DY_NOEXCEPT
{
    assert(str != nullptr);

    return new _dy_val_t {
        .type = dy_type_str,
        .data = { .str = std::string(str) },
    };
}

DY_GET_LEN(str);

DY_PUBLIC(char const*) dy_get_str_data(dy_t val) DY_NOEXCEPT
{
    DY_ASSERT(str);
    return DY_DATA(str).c_str();
}

// ---------------------------------- barr ---------------------------------- //

DY_MAKE_LEN(barr) DY_GET_LEN(barr) DY_GET_IDX(barr);

// ---------------------------------- bytes --------------------------------- //

DY_MAKE_LEN(bytes) DY_GET_LEN(bytes) DY_GET_DATA(bytes) DY_GET_IDX(bytes);

// ---------------------------------- iarr ---------------------------------- //

DY_MAKE_LEN(iarr) DY_GET_LEN(iarr) DY_GET_DATA(iarr) DY_GET_IDX(iarr);

// ---------------------------------- farr ---------------------------------- //

DY_MAKE_LEN(farr) DY_GET_LEN(farr) DY_GET_DATA(farr) DY_GET_IDX(farr);

// ---------------------------------- arr  ---------------------------------- //

DY_MAKE_LEN(arr) DY_GET_LEN(arr) DY_GET_DATA(arr) DY_GET_IDX(arr);

// ---------------------------------- map  ---------------------------------- //

DY_PUBLIC(dy_t) dy_make_map(dy_keyval_t const* ptr, size_t len) DY_NOEXCEPT
{
    unordered_map<string, dy_t> map;
    map.reserve(len);
    for (size_t i = 0; i < len; ++i)
    {
        dy_keyval_t const& pair = ptr[i];
        map.insert(make_pair(string(pair.key), pair.val));
    }
    return new _dy_val_t {
        .type = dy_type_map,
        .data = { .map = move(map) },
    };
}

DY_GET_LEN(map);

DY_PUBLIC(dy_iter_t) dy_make_map_iter(dy_t val) DY_NOEXCEPT
{
    DY_ASSERT(map);
    return new _dy_iter_t { .it = DY_DATA(map).begin() };
}

DY_PUBLIC(dy_keyval_t)
dy_get_map_iter(dy_t val, dy_iter_t iter) DY_NOEXCEPT
{
    DY_ASSERT(map);
    assert(iter != nullptr);

    auto& it = iter->it;

    if (it != DY_DATA(map).end())
    {
        auto const& pair = *it;
        ++it;

        return dy_keyval_t {
            .key = pair.first.c_str(),
            .val = pair.second,
        };
    }
    else
    {
        return dy_keyval_t {
            .key = nullptr,
            .val = nullptr,
        };
    }
}

DY_PUBLIC(void) dy_dispose_map_iter(dy_iter_t iter) DY_NOEXCEPT
{
    delete iter;
}

DY_PUBLIC(dy_keyval_t) dy_get_map_key(dy_t val, char const* key) DY_NOEXCEPT
{
    DY_ASSERT(map);
    assert(key != nullptr);

    auto const& data = DY_DATA(map);

    if (auto it = data.find(key); it != data.end())
    {
        auto const& pair = *it;
        return dy_keyval_t {
            .key = pair.first.c_str(),
            .val = pair.second,
        };
    }
    else
    {
        return dy_keyval_t {
            .key = nullptr,
            .val = nullptr,
        };
    }
}