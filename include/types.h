#pragma once

#if defined(__INT8_TYPE__) && defined(__INT16_TYPE__) && defined(__INT32_TYPE__) && defined(__INT64_TYPE__) && \
    defined(__UINT8_TYPE__) && defined(__UINT16_TYPE__) && defined(__UINT32_TYPE__) && defined(__UINT64_TYPE__)

#define INTN_TYPE_DEFINED

typedef __INT8_TYPE__       i8;
typedef __INT16_TYPE__      i16;
typedef __INT32_TYPE__      i32;
typedef __INT64_TYPE__      i64;

typedef __UINT8_TYPE__      u8;
typedef __UINT16_TYPE__     u16;
typedef __UINT32_TYPE__     u32;
typedef __UINT64_TYPE__     u64;

#else

#include <stdint.h>

typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;

typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef uint64_t            u64;

#endif

#ifndef __cplusplus
#ifdef __WCHAR_TYPE__
typedef __WCHAR_TYPE__ wchar_t;
#else
#include <stddef.h>
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 202000L && \
    !defined(bool)
typedef _Bool bool;
#define true ((bool) 1)
#define false ((bool) 0)
#endif
#endif

//
// float typedefs
//

#if defined(__SIZEOF_FLOAT__)   && \
    defined(__SIZEOF_DOUBLE__)  && \
    defined(__SIZEOF_LONG_DOUBLE__) && \
    defined(__FLT_DIG__) && \
    defined(__DBL_DIG__) && \
    defined(__LDBL_DIG__)


#if __SIZEOF_DOUBLE__ == 4

#define f32_digits __DBL_DIG__
#define f32_max __DBL_MAX__

typedef double f32;

#elif __SIZEOF_LONG_DOUBLE__ == 4

#define f32_digits __LDBL_DIG__
#define f32_max __LDBL_MAX__

typedef long double f32;

#else

#define f32_digits __FLT_DIG__
#define f32_max __FLT_MAX__

typedef float f32;

#endif


#if __SIZEOF_FLOAT__ == 8

#define f64_digits __FLT_DIG__
#define f64_max __FLT_MAX__

typedef float f64;

#elif __SIZEOF_DOUBLE__ == 8

#define f64_digits __DBL_DIG__
#define f64_max __DBL_MAX__

typedef double f64;

#else

#define f64_digits __LDBL_DIG__
#define f64_max __LDBL_MAX__

typedef long double f64;

#endif


#define fmax_digits __LDBL_DIG__
#define fmax_max __LDBL_MAX__

#else

#include <float.h>

#if DBL_MANT_DIG == 24
#define f32_digits DBL_DIG
#define f32_max DBL_MAX

typedef double f32;

#elif LDBL_MANT_DIG == 24

#define f32_digits LDBL_DIG
#define f32_max LDBL_MAX

typedef long double f32;

#else

#define f32_digits FLT_DIG
#define f32_max FLT_MAX

typedef float f32;

#endif


#if FLT_MANT_DIG == 53
#define f64_digits FLT_DIG
#define f64_max FLT_MAX

typedef float f64;

#elif DBL_MANT_DIG == 53

#define f64_digits DBL_DIG
#define f64_max DBL_MAX

typedef double f64;

#else

#define f64_digits LDBL_DIG
#define f64_max LDBL_MAX

typedef long double f64;

#endif

#define fmax_digits LDBL_DIG
#define fmax_max LDBL_MAX

#endif

typedef long double fmax_t;

//
// constants for 10^digits in floating point types
//

#define _concat1(a, b) a ## b
#define _concat(a, b) _concat1(a, b)

#define f32_multiplier  _concat(1e+, f32_digits)
#define f64_multiplier  _concat(1e+, f64_digits)
#define fmax_multiplier _concat(1e+, fmax_digits)


//
// integer limits
//

#define u8_max  ((u8)-1)
#define u16_max ((u16)-1)
#define u32_max ((u32)-1)
#define u64_max ((u64)-1)

#define i8_max  ((i8)(u8_max >> 1))
#define i16_max ((i16)(u16_max >> 1))
#define i32_max ((i32)(u32_max >> 1))
#define i64_max ((i64)(u64_max >> 1))

//
// misc
//



#ifdef __cplusplus
#define __constexpr constexpr
#define __constexpr_if if constexpr
#define __constexpr_function constexpr

template <typename T>
inline T *to_ptr(const T &lvalue)
    { return const_cast<T*>(&lvalue); }

#else
#define __constexpr const
#define __constexpr_if if
#define __constexpr_function inline
#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__) \
 || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L)
#define to_ptr(lvalue) ((typeof(lvalue)[1]){lvalue})
#else
#define to_ptr(lvalue) ((decltype(lvalue)[1]){lvalue})
#endif
#endif


//
// Check if float types are the same
// - they shouldn't be.
//

#if f32_digits == f64_digits || f32_digits == fmax_digits \
 || f64_digits == fmax_digits
#pragma error "Float types are the same."
#endif
