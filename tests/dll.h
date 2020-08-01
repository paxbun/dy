// Copyright (c) 2020 Chanjung Kim (paxbun). All rights reserved.

#ifndef DY_TEST_DLL_H
#define DY_TEST_DLL_H

#include <dy.h>

#ifdef __cplusplus
#    define DLL_EXT      extern "C"
#    define DLL_NOEXCEPT noexcept
#else
#    define DLL_EXT extern
#    define DLL_NOEXCEPT
#endif

#ifdef _WIN32
#    ifdef DLL_EXPORT
#        define DLL_PUBLIC(ty) DLL_EXT __declspec(dllexport) ty
#    else
#        define DLL_PUBLIC(ty) DLL_EXT __declspec(dllimport) ty
#    endif
#else
#    if __GNUC__ >= 4
#        define DLL_PUBLIC(ty) DLL_EXT __attribute__((visibility("default"))) ty
#    else
#        define DLL_PUBLIC(ty) DLL_EXT ty
#    endif
#endif

DLL_PUBLIC(dy_t) get_sample_generic_barr() DY_NOEXCEPT;

DLL_PUBLIC(dy_t) get_sample_generic_iarr() DY_NOEXCEPT;

DLL_PUBLIC(dy_t) get_sample_generic_farr() DY_NOEXCEPT;

DLL_PUBLIC(dy_t) get_sample_generic_array() DY_NOEXCEPT;

DLL_PUBLIC(dy_t) get_sample_generic_map() DY_NOEXCEPT;

#endif