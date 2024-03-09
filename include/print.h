#pragma once

#include <stdio.h>
#include "types.h"
#include "null.h"


#ifdef __cplusplus
extern "C"
{
#endif

//
// For special prints,
// e.g. print(showpos)
// would enable positive
// sign being shown.
//
// So 12 would become +12 instead etc.
//

static bool __showpos = false;

typedef struct print_sp_t
{
    u8 ios_code;
} print_sp_t;

#define SHOWPOS_CODE    0
#define NOSHOWPOS_CODE  1

static const print_sp_t showpos = {SHOWPOS_CODE};
static const print_sp_t noshowpos = {NOSHOWPOS_CODE};

#define print_type void
#define print_htype static inline print_type


print_htype
print_sp(FILE *file, print_sp_t special)
{
    switch (special.ios_code)
    {
        case SHOWPOS_CODE:
        {
            __showpos = true;
            break;
        }
        case NOSHOWPOS_CODE:
        {
            __showpos = false;
            break;
        }
    }
}


print_htype
print_ch_ptr(FILE *file, const char *s)
{
    if (s == null)
    {
        print_ch_ptr(file, "(null_t)null");
        return;
    }

    for (const char *save = s; *save; save++)
    {
        putc(*save, file);
    }
}

print_htype
print_ptr(FILE *file, void *ptr)
{
    putc('0', file);
    putc('x', file);

    const char *const hex_chars = "0123456789abcdef";

    for (i8 i = (2 * sizeof(ptr) - 1); i >= 0; i--)
    {
        putc(hex_chars[(((u64) ptr) >> (i * 4)) & 0xF], file);
    }
}

//
// |-----------------------------|
// | Overloads for integer types |
// |-----------------------------|
//

void print_x16(FILE *file, u16 n);
void print_x32(FILE *file, u32 n);
void print_x64(FILE *file, u64 n);


print_htype
print_u16(FILE *file, u16 n)
{
    if (__showpos)
    {
        putc('+', file);
    }

    print_x16(file, n);
}

print_htype
print_u32(FILE *file, u32 n)
{
    if (__showpos)
    {
        putc('+', file);
    }

    print_x32(file, n);
}

print_htype
print_u64(FILE *file, u64 n)
{
    if (__showpos)
    {
        putc('+', file);
    }

    print_x64(file, n);
}

print_htype
print_i16(FILE *file, i16 n)
{
    if (n < 0)
    {
        putc('-', file);
        n = -n;
    }
    else if (__showpos)
    {
        putc('+', file);
    }

    print_x16(file, n);
}

print_htype
print_i32(FILE *file, i32 n)
{
    if (n < 0)
    {
        putc('-', file);
        n = -n;
    }
    else if (__showpos)
    {
        putc('+', file);
    }

    print_x32(file, n);
}

print_htype
print_i64(FILE *file, i64 n)
{
    if (n < 0)
    {
        putc('-', file);
        n = -n;
    }
    else if (__showpos)
    {
        putc('+', file);
    }

    print_x64(file, n);
}

void print_f32(FILE *file, f32 f);
void print_f64(FILE *file, f64 f);
void print_fmax(FILE *file, fmax_t f);

print_htype
print_ch(FILE *file, char c)
{
    putc(c, file);
}

print_htype
print_bool(FILE *file, bool b)
{
    print_ch_ptr(file, b ? "true" : "false");
}

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus

//
// Weird implementation to mimic C++ overloads.
// To overload again do:
// 
//  #undef generic_fprint_impl
//  #define generic_fprint_impl(f, s) (_Generic((s), /* insert types and relative functions here*/, default: _generic_default_fprint))
// 
// Note: This only works once... Will probably have to try and figure something else out.
// TODO: fix pls.
//

#define _generic_default_fprint(s) (_Generic((s), \
    const   char*: print_ch_ptr,        \
            char*: print_ch_ptr,        \
            char: print_ch,             \
            u16: print_u16,             \
            u32: print_u32,             \
            u64: print_u64,             \
            i16: print_i16,             \
            i32: print_i32,             \
            i64: print_i64,             \
            f32: print_f32,             \
            f64: print_f64,             \
            fmax_t: print_fmax,         \
            bool: print_bool,           \
            print_sp_t: print_sp,       \
            default: print_ptr          \
))

#define generic_fprint_impl(f, s) _generic_default_fprint(s)(f, s)
#define fprint(file, str) generic_fprint_impl(file, str)
#define fprintln(file, str) ({ fprint(file, str); putc('\n', file); })

#define endl ((char)'\n')

#else

print_htype fprint(
    FILE *file, const char *str)
{ print_ch_ptr(file, str); }

print_htype fprint(
    FILE *file, char *str)
{ print_ch_ptr(file, str); }

print_htype fprint(
    FILE *file, char c)
{ print_ch(file, c); }

print_htype fprint(
    FILE *file, u16 n)
{ print_u16(file, n); }

print_htype fprint(
    FILE *file, u32 n)
{ print_u32(file, n); }

print_htype fprint(
    FILE *file, u64 n)
{ print_u64(file, n); }

print_htype fprint(
    FILE *file, i16 n)
{ print_i16(file, n); }

print_htype fprint(
    FILE *file, i32 n)
{ print_i32(file, n); }

print_htype fprint(
    FILE *file, i64 n)
{ print_i64(file, n); }

print_htype fprint(
    FILE *file, f32 f)
{ print_f32(file, f); }

print_htype fprint(
    FILE *file, f64 f)
{ print_f64(file, f); }

print_htype fprint(
    FILE *file, fmax_t f)
{ print_fmax(file, f); }

print_htype fprint(
    FILE *file, void *ptr)
{ print_ptr(file, ptr); }

template<typename T>
print_htype fprintln(
    FILE *file, T x)
{
    fprint(file, x);
    fprint(file, '\n');
}

#endif


#define print(str) fprint(stdout, str)
#define println(str) fprintln(stdout, str)
