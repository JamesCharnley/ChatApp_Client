///
/// @file Defines.h
///
/// @brief Common platform definitions
///
/// @author
///
/// This file is a part of Ultralight, a next-generation HTML renderer.
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2022 Ultralight, Inc. All rights reserved.
///
#pragma once

// Needed for limit defines, like INTMAX_MAX, which is used by the std C++ library
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <cstdint>
#include <cstddef>
#include <limits.h>

#ifdef SWIG
#define UExport
#else

// Require C++11 Support
#if defined(_MSC_VER)
#   if _MSC_VER < 1800 
#       error This project needs at least Visual Studio 2013 to build
#   endif
#elif __cplusplus <= 199711L
#   error This project can only be compiled with a compiler that supports C++11
#endif

#if INTPTR_MAX == INT32_MAX
#define UL_ARCH_32_BIT
#elif INTPTR_MAX == INT64_MAX
#define UL_ARCH_64_BIT
#else
#error "Unknown CPU architecture: environment not 32 or 64-bit."
#endif

#if defined(__aarch64__)
#  define UL_ARCH_ARM64
#  if defined(__APPLE__)
#    define UL_ARCH_ARM64_APPLE_SILICON
#  endif
#endif

#if defined(__WIN32__) || defined(_WIN32)
#  if defined(ULTRALIGHT_STATIC_BUILD)
#    define UExport 
#  else
#    if defined(ULTRALIGHT_IMPLEMENTATION)
#      define UExport __declspec(dllexport)
#    else
#      define UExport __declspec(dllimport)
#    endif
#  endif
#define _thread_local __declspec(thread)
#ifndef _NATIVE_WCHAR_T_DEFINED
#define DISABLE_NATIVE_WCHAR_T
#endif
#else
#  if defined(ULTRALIGHT_STATIC_BUILD)
#    define UExport 
#  else
#    define UExport __attribute__((visibility("default")))
#  endif
#define _thread_local __thread
#endif

#endif

#define ULTRALIGHT_VERSION "1.3.0"
#define ULTRALIGHT_VERSION_MAJOR 1
#define ULTRALIGHT_VERSION_MINOR 3
#define ULTRALIGHT_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

UExport const char* UltralightVersionString();
UExport uint32_t UltralightVersionMajor();
UExport uint32_t UltralightVersionMinor();
UExport uint32_t UltralightVersionPatch();

#ifdef __cplusplus
}
#endif

///
/// @mainpage Ultralight C++ API Reference
///
/// @section intro_sec Introduction
///
/// Hi there, welcome to the C++ API Reference for Ultralight!
///
/// Ultralight is a next-generation HTML renderer for desktop apps and games.
///
/// If this is your first time exploring the API, we recommend starting with Renderer.h and View.h.
///
///
/// @section usefullinks_sec Useful Links
/// - Home:     <https://ultralig.ht> -- Get the latest binaries
/// - Docs:     <https://docs.ultralig.ht> -- API overview, code snippets, tutorials and more!
/// - Discord:  <http://chat.ultralig.ht/> -- Stuck? Have questions? Come chat with us!
/// - GitHub:   <https://github.com/ultralight-ux/ultralight> -- Report issues and browse code
///
/// @section copyright_sec Copyright
/// Documentation is Copyright (C) 2022 Ultralight, Inc. All rights reserved.
///
