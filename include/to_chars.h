#pragma once

#include <math.h>
#include "throw.h"
#include "types.h"
#include "null.h"


//
// __inline_or_constexpr_to_chars should be used for any function
// in to_chars that can be constexpr.
//
// Uses static inline instead if before C++14 to satisfy:
//  https://en.cppreference.com/w/cpp/language/constexpr
//
// In the `constexpr function (until C++14)` section.
//

#if defined(__cplusplus) && __cplusplus >= 201400LL
#define __inline_or_constexpr_to_chars constexpr
#else
#define __inline_or_constexpr_to_chars static inline
#endif

typedef struct to_chars_result
{
    char *ptr;
    error_type errc;
} to_chars_result;


//
//  to_chars
// what was this comment supposed to do?
//


to_chars_result
_f32_to_chars(
    char *first,
    char *const last,
    const f32 value);

to_chars_result
_f64_to_chars(
    char *first,
    char *const last,
    const f64 value);

to_chars_result
_fmax_to_chars(
    char *first,
    char *const last,
    const fmax_t value);


__inline_or_constexpr_to_chars
to_chars_result
_u16_to_chars(
    char *first,
    char *const last,
    const u16 value)
{
    if (value < 10)
    {
        if (last - first <= 0)
            return (to_chars_result){last, out_of_range};
        
        *first++ = '0' + value;
        return (to_chars_result){first, success};
    }

    u16 save = value;
    u8 len = 1;

    for (; (save /= 10) >= 10; len++) {}

    if (last - first <= len)
        return (to_chars_result){last, out_of_range};

    *first++ = '0' + save;

    first += len;

    save = value;

    for (u8 i = 1; i <= len; i++)
    {
        first[-i] = '0' + (save % 10);
        save /= 10;
    }

    return (to_chars_result){first, success};
}

__inline_or_constexpr_to_chars
to_chars_result
_u32_to_chars(
    char *first,
    char *const last,
    const u32 value)
{
    if (value < 10)
    {
        if (last - first <= 0)
            return (to_chars_result){last, out_of_range};
        
        *first++ = '0' + value;
        return (to_chars_result){first, success};
    }

    u32 save = value;
    u8 len = 1;

    for (; (save /= 10) >= 10; len++) {}

    if (last - first <= len)
        return (to_chars_result){last, out_of_range};

    *first++ = '0' + save;

    first += len;

    save = value;

    for (u8 i = 1; i <= len; i++)
    {
        first[-i] = '0' + (save % 10);
        save /= 10;
    }

    return (to_chars_result){first, success};
}

__inline_or_constexpr_to_chars
to_chars_result
_u64_to_chars(
    char *first,
    char *const last,
    const u64 value)
{
    if (value < 10)
    {
        if (last - first <= 0)
            return (to_chars_result){last, out_of_range};
        
        *first++ = '0' + value;
        return (to_chars_result){first, success};
    }

    u64 save = value;
    u8 len = 1;

    for (; (save /= 10) >= 10; len++) {}

    if (last - first <= len)
        return (to_chars_result){last, out_of_range};

    *first++ = '0' + save;

    first += len;

    save = value;

    for (u8 i = 1; i <= len; i++)
    {
        first[-i] = '0' + (save % 10);
        save /= 10;
    }

    return (to_chars_result){first, success};
}


__inline_or_constexpr_to_chars
to_chars_result
_i16_to_chars(
    char *first,
    char *const last,
    const i16 value)
{
    if (first >= last)
        return (to_chars_result){last, out_of_range};

    if (value < 0)
    {
        *first++ = '-';

        return _u16_to_chars(
            first, last, (u16)(-value));
    }

    return _u16_to_chars(
        first, last, (u16)value);
}

__inline_or_constexpr_to_chars
to_chars_result
_i32_to_chars(
    char *first,
    char *const last,
    const i32 value)
{
    if (first >= last)
        return (to_chars_result){last, out_of_range};
    
    if (value < 0)
    {
        *first++ = '-';

        return _u32_to_chars(
            first, last, (u32)(-value));
    }

    return _u32_to_chars(
        first, last, (u32)value);
}

__inline_or_constexpr_to_chars
to_chars_result
_i64_to_chars(
    char *first,
    char *const last,
    const i64 value)
{
    if (first >= last)
        return (to_chars_result){last, out_of_range};
    
    if (value < 0)
    {
        *first++ = '-';

        return _u64_to_chars(
            first, last, (u64)(-value));
    }

    return _u64_to_chars(
        first, last, (u64)value);
}

#ifndef __cplusplus

#define to_chars(first, last, value) _Generic((value),  \
    f32: _f32_to_chars,                                 \
    f64: _f64_to_chars,                                 \
    fmax_t: _fmax_to_chars,                             \
    u16: _u16_to_chars,                                 \
    u32: _u32_to_chars,                                 \
    u64: _u64_to_chars,                                 \
    i16: _i16_to_chars,                                 \
    i32: _i32_to_chars,                                 \
    i64: _i64_to_chars)(first, last, value)

#else

static inline
to_chars_result to_chars(
    char *first,
    char *const last,
    const f32 value)
{ return _f32_to_chars(first, last, value); }

static inline
to_chars_result to_chars(
    char *first,
    char *const last,
    const f64 value)
{ return _f64_to_chars(first, last, value); }

static inline
to_chars_result to_chars(
    char *first,
    char *const last,
    const fmax_t value)
{ return _fmax_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const u16 value)
{ return _u16_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const u32 value)
{ return _u32_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const u64 value)
{ return _u64_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const i16 value)
{ return _i16_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const i32 value)
{ return _i32_to_chars(first, last, value); }

__inline_or_constexpr_to_chars
to_chars_result to_chars(
    char *first,
    char *const last,
    const i64 value)
{ return _i64_to_chars(first, last, value); }

#endif


//
// TODO:
//  from_chars
//



#define fdigits f32_digits
#define fmultiplier f32_multiplier
