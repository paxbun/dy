// Copyright (c) 2020 Stella Authors. All rights reserved.

#ifndef DY_H
#define DY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#    define DY_EXT      extern "C"
#    define DY_NOEXCEPT noexcept
#else
#    define DY_EXT extern
#    define DY_NOEXCEPT
#endif

#ifdef _WIN32
#    ifdef DY_EXPORT
#        define DY_PUBLIC(ty) DY_EXT __declspec(dllexport) ty
#    else
#        define DY_PUBLIC(ty) DY_EXT __declspec(dllimport) ty
#    endif
#else
#    if __GNUC__ >= 4
#        define DY_PUBLIC(ty) DY_EXT __attribute__((visibility("default"))) ty
#    else
#        define DY_PUBLIC(ty) DY_EXT ty
#    endif
#endif

#define DY_DEF_MAKE(f, ty) DY_PUBLIC(dy_t) dy_make_##f(ty f) DY_NOEXCEPT

#define DY_DEF_GET(f, ty) DY_PUBLIC(ty) dy_get_##f(dy_t val) DY_NOEXCEPT

#define DY_DEF_MAKE_LEN(f, ty)                                                 \
    DY_PUBLIC(dy_t) dy_make_##f(ty const* f, size_t len) DY_NOEXCEPT

#define DY_DEF_GET_LEN(f)                                                      \
    DY_PUBLIC(size_t) dy_get_##f##_len(dy_t val) DY_NOEXCEPT

#define DY_DEF_GET_DATA(f, ty)                                                 \
    DY_PUBLIC(ty const*) dy_get_##f##_data(dy_t val) DY_NOEXCEPT

#define DY_DEF_GET_IDX(f, ty)                                                  \
    DY_PUBLIC(ty) dy_get_##f##_idx(dy_t val, size_t idx) DY_NOEXCEPT

/// <summary>
/// indicates the type of the value
/// </summary>
typedef enum _dy_type_t
{
    /// <summary>
    /// null
    /// </summary>
    dy_type_null,

    /// <summary>
    /// boolean
    /// </summary>
    dy_type_b,

    /// <summary>
    /// 8-byte integer
    /// </summary>
    dy_type_i,

    /// <summary>
    /// double-precision floating-point number
    /// </summary>
    dy_type_f,

    /// <summary>
    /// UTF-8 string
    /// </summary>
    dy_type_str,

    /// <summary>
    /// boolean array
    /// </summary>
    dy_type_barr,

    /// <summary>
    /// byte array
    /// </summary>
    dy_type_bytes,

    /// <summary>
    /// integer array
    /// </summary>
    dy_type_iarr,

    /// <summary>
    /// floating-point number array
    /// </summary>
    dy_type_farr,

    /// <summary>
    /// generic array
    /// </summary>
    dy_type_arr,

    /// <summary>
    /// generic map
    /// </summary>
    dy_type_map,
} dy_type_t;

/// <summary>
/// indicates a value
/// </summary>
typedef struct _dy_val_t* dy_t;

/// <summary>
/// indicates a key-value pair
/// </summary>
typedef struct _dy_keyval_t
{
    /// <summary>
    /// the key
    /// </summary>
    char const* key;

    /// <summary>
    /// the value
    /// </summary>
    dy_t val;
} dy_keyval_t;

/// <summary>
/// indicates an iterator of a generic map
/// </summary>
typedef struct _dy_iter_t* dy_iter_t;

/// <summary>
/// returns the type of the value
/// </summary>
/// <param name="val">the value to get the type of</param>
/// <returns>the type of the value</returns>
DY_PUBLIC(dy_type_t) dy_get_type(dy_t val) DY_NOEXCEPT;

/// <summary>
/// deepcopies the value
/// </summary>
/// <param name="val">the value to copy</param>
/// <returns>newly created value instance</returns>
DY_PUBLIC(dy_t) dy_copy(dy_t val) DY_NOEXCEPT;

/// <summary>
/// deallocates the memory holding the instance and the internal data
/// </summary>
/// <param name="val">the instance</param>
DY_PUBLIC(void) dy_dispose(dy_t val) DY_NOEXCEPT;

/// <summary>
/// deallocates the memory holding the instance but not the internal data.
/// </summary>
/// <param name="val">the instance</param>
DY_PUBLIC(void) dy_dispose_self(dy_t val) DY_NOEXCEPT;

// ---------------------------------- null ---------------------------------- //

/// <summary>
/// makes a null value
/// </summary>
/// <returns>a new value instance</returns>
DY_PUBLIC(dy_t) dy_make_null() DY_NOEXCEPT;

// ----------------------------------- b  ----------------------------------- //

/// <summary>
/// makes a boolean value
/// </summary>
/// <param name="b">internal data</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE(b, bool);

/// <summary>
/// checks if the value is boolean and returns the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the internal data</returns>
DY_DEF_GET(b, bool);

// ----------------------------------- i  ----------------------------------- //

/// <summary>
/// makes an integer value
/// </summary>
/// <param name="i">internal data</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE(i, int64_t);

/// <summary>
/// checks if the value is an integer and returns the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the internal data</returns>
DY_DEF_GET(i, int64_t);

// ----------------------------------- f  ----------------------------------- //

/// <summary>
/// makes a double-precision number value
/// </summary>
/// <param name="i">internal data</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE(f, double);

/// <summary>
/// checks if the value is a double-precision number value and returns the
/// internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the internal data</returns>
DY_DEF_GET(f, double);

// ---------------------------------- str  ---------------------------------- //

/// <summary>
/// makes a string value
/// </summary>
/// <param name="str">a pointer to the string to copy</param>
/// <param name="len">the length of the string</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE(str, char const*);

/// <summary>
/// returns the length of the string in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the lenght of the string</returns>
DY_DEF_GET_LEN(str);

/// <summary>
/// returns the pointer to the string in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the pointer to the string</returns>
DY_DEF_GET_DATA(str, char);

// ---------------------------------- barr ---------------------------------- //

/// <summary>
/// makes a boolean array value
/// </summary>
/// <param name="barr">a pointer to the boolean array to copy</param>
/// <param name="len">the length of the array</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE_LEN(barr, bool);

/// <summary>
/// returns the length of the boolean array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the boolean array</returns>
DY_DEF_GET_LEN(barr);

/// <summary>
/// returns the value of a entry at the given index of the boolean array in the
/// internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="idx">the index of the entry</param>
/// <returns>the value of the entry</returns>
DY_DEF_GET_IDX(barr, bool);

// --------------------------------- bytes  --------------------------------- //

/// <summary>
/// makes an byte array value
/// </summary>
/// <param name="bytes">a pointer to the byte array to copy</param>
/// <param name="len">the length of the array</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE_LEN(bytes, uint8_t);

/// <summary>
/// returns the length of the byte array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the byte array</returns>
DY_DEF_GET_LEN(bytes);

/// <summary>
/// returns the pointer to the byte array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the pointer to the byte array</returns>
DY_DEF_GET_DATA(bytes, uint8_t);

/// <summary>
/// returns the value of a entry at the given index of the byte array in the
/// internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="idx">the index of the entry</param>
/// <returns>the value of the entry</returns>
DY_DEF_GET_IDX(bytes, uint8_t);

// ---------------------------------- iarr ---------------------------------- //

/// <summary>
/// makes an integer array value
/// </summary>
/// <param name="iarr">a pointer to the integer array to copy</param>
/// <param name="len">the length of the array</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE_LEN(iarr, int64_t);

/// <summary>
/// returns the length of the integer array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the integer array</returns>
DY_DEF_GET_LEN(iarr);

/// <summary>
/// returns the pointer to the integer array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the pointer to the integer array</returns>
DY_DEF_GET_DATA(iarr, int64_t);

/// <summary>
/// returns the value of a entry at the given index of the integer array in the
/// internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="idx">the index of the entry</param>
/// <returns>the value of the entry</returns>
DY_DEF_GET_IDX(iarr, int64_t);

// ---------------------------------- farr ---------------------------------- //

/// <summary>
/// makes an double-precision number array value
/// </summary>
/// <param name="farr">a pointer to the double-precision number array to
/// copy</param> <param name="len">the length of the array</param> <returns>a
/// new value instance</returns>
DY_DEF_MAKE_LEN(farr, double);

/// <summary>
/// returns the length of the double-precision number array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the double-precision number array</returns>
DY_DEF_GET_LEN(farr);

/// <summary>
/// returns the pointer to the double-precision number array in the internal
/// data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the pointer to the double-precision number array</returns>
DY_DEF_GET_DATA(farr, double);

/// <summary>
/// returns the value of a entry at the given index of the double-precision
/// number array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="idx">the index of the entry</param>
/// <returns>the value of the entry</returns>
DY_DEF_GET_IDX(farr, double);

// ---------------------------------- arr  ---------------------------------- //

/// <summary>
/// makes a generic array value
/// </summary>
/// <param name="arr">a pointer to the generic array to copy</param>
/// <param name="len">the length of the array</param>
/// <returns>a new value instance</returns>
DY_DEF_MAKE_LEN(arr, dy_t);

/// <summary>
/// returns the length of the generic array in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the generic array</returns>
DY_DEF_GET_LEN(arr);

/// <summary>
/// returns the pointer to the generic array in the internal
/// data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the pointer to the generic array</returns>
DY_DEF_GET_DATA(arr, dy_t);

/// <summary>
/// returns the value of a entry at the given index of the generic array in the
/// internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="idx">the index of the entry</param>
/// <returns>the value of the entry</returns>
DY_DEF_GET_IDX(arr, dy_t);

// ---------------------------------- map  ---------------------------------- //

/// <summary>
/// makes a generic map
/// </summary>
/// <param name="map">a pointer to the key-value pair array to copy</param>
/// <param name="len">the length of the array</param>
/// <returns></returns>
DY_DEF_MAKE_LEN(map, dy_keyval_t);

/// <summary>
/// returns the length of the generic map in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>the length of the generic map</returns>
DY_DEF_GET_LEN(map);

/// <summary>
/// makes an iterator of the generic map in the internal data
/// </summary>
/// <param name="val">the value instance</param>
/// <returns>a new iterator instance</returns>
DY_PUBLIC(dy_iter_t) dy_make_map_iter(dy_t val) DY_NOEXCEPT;

/// <summary>
/// returns the data pointed by the given iterator
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="iter">the iterator instance</param>
/// <param name="keyval">the pointer to copy value</param>
/// <returns>the key-value pair pointed</returns>
DY_PUBLIC(dy_keyval_t) dy_get_map_iter(dy_t val, dy_iter_t iter) DY_NOEXCEPT;

/// <summary>
/// deallocates the memory holding the iterator instance
/// </summary>
/// <param name="iter">the iterator</param>
DY_PUBLIC(void) dy_dispose_map_iter(dy_iter_t iter) DY_NOEXCEPT;

/// <summary>
/// returns the data with the given key
/// </summary>
/// <param name="val">the value instance</param>
/// <param name="key">the key</param>
/// <returns>the key-value pair with the given key</returns>
DY_PUBLIC(dy_keyval_t) dy_get_map_key(dy_t val, char const* key) DY_NOEXCEPT;

#endif