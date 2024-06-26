#pragma once

#include "types.h"
#include "throw.h"
#include "print.h"
#include "null.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Adds SAL support.
//
// Only defines needed macros if sal.h isn't available.
//

#if defined(_MSC_VER)
#include <sal.h>
#else

#ifndef _In_
#define _In_
#endif

#ifndef _Out_
#define _Out_
#endif

#ifndef _Inout_
#define _Inout_
#endif

#ifndef _In_opt_
#define _In_opt_
#endif

#endif

//
// Check if all macros are already defined.
//
// If they are defined, undefine them so
// that they can be redefined without
// causing any errors.
//

#ifdef STRING_SHORT_MAX
#undef STRING_SHORT_MAX
#endif

#ifdef STRING_MAX_DIGITS
#undef STRING_MAX_DIGITS
#endif

#ifdef npos
#undef npos
#endif


typedef char string_value_type;

typedef string_value_type*       string_iterator;
typedef const string_value_type* const_string_iterator;
typedef u32                      string_size_type;


//
// To keep the sizeof(string) constant if the
// sizeof(string_value_type) is not 1 (string
// usually uses char as the data type).
//
// Max size of string::__short_data_[].
//

#define STRING_SHORT_MAX (24 / sizeof(string_value_type))

//
// As a return value, it is usually used to indicate no matches.
// When used as a parameter for String functions, it means
// "until the end of the string".
//

static const string_size_type npos = -1;

typedef struct string
{
    union
    {
        string_value_type __short_data_[STRING_SHORT_MAX];
        
        //
        // Data stored here past STRING_SHORT_MAX characters
        //

        string_value_type *__long_data_;
    };

    string_size_type capacity, length;

#ifdef __cplusplus
    constexpr inline string(void) noexcept
        : __short_data_{}, capacity(STRING_SHORT_MAX), length(0) {}
    
    inline string(const string &rhs);
    inline string(const string_value_type *s);
#endif
} string;


//
// Definitions for String functions and
// other misc.
//

//
// Changed these to typedefs for each function,
// I don't know why but intelisense seems to prefer
// as some were getting doxygen comments showing
// whereas others weren't.
//

typedef string_iterator (*string_begin_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef string_iterator (*string_end_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef string (*string_init_fun)(
        _In_opt_ const string_value_type *s);

typedef void (*string_destroy_fun)(
        _Inout_ string  __notnullptr(str))
        __notnull(1);

typedef void (*string_reserve_fun)(
        _Inout_ string              __notnullptr(str),
                string_size_type    n)
        __notnull(1);

typedef void (*string_append_fun)(
        _Inout_ string                  __notnullptr(str),
        _In_    const string_value_type __notnullptr(s))
        __notnull(1) __notnull(2);

typedef void (*string_clear_fun)(
        _Inout_ string  __notnullptr(str))
        __notnull(1);

typedef void (*string_resize_fun)(
        _Inout_ string              __notnullptr(str),
                string_size_type    size)
        __notnull(1);

// lol the name is so long
typedef void (*string_shrink_to_fit_fun)(
        _Inout_ string  __notnullptr(str))
        __notnull(1);

typedef string (*string_substr_fun)(
        _In_    string              __notnullptr(str),
                string_size_type    pos,
                string_size_type    len)
        __notnull(1);

typedef string (*string_copy_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef void (*string_insert_fun)(
        _Inout_ string                  __notnullptr(str),
                string_size_type        pos,
        _In_    const string_value_type __notnullptr(s))
        __notnull(1) __notnull(3);

typedef void (*string_push_back_fun)(
        _Inout_ string              __notnullptr(str),
                string_value_type   c)
        __notnull(1);

typedef string_value_type *(*string_data_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef const string_value_type *(*string_c_str_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef bool (*string_empty_fun)(
        _In_    string  __notnullptr(str))
        __notnull(1);

typedef void (*string_inserts_fun)(
        _Inout_ string              __notnullptr(dest),
                string_size_type    pos,
        _In_    string              __notnullptr(src))
        __notnull(1) __notnull(3);

//
// struct storing most functions
// to manipulate strings.
//
// Mainly for prettiness but also so
// it has a "namespace", which prevents
// name clashes and means the functions
// can be named nicer.
//

typedef struct $String
{
    /// @brief Returns an iterator pointing to the first character of the string.
    /// @param str Pointer to string object.
    /// @return An iterator to the beginning of the string.
    string_begin_fun begin;

    /// @brief Returns an iterator pointing to the past-the-end character of the string.
    /// @param str Pointer to string object.
    /// @return An iterator to the past-the-end of the string.
    /// @note - The past-the-end character is a theoretical character
    /// @note   that would follow the last character in the string. It
    /// @note   shall not be dereferenced.
    /// @note - If the object is an empty string, this function returns the same as string::begin.
    string_end_fun end;


    /// @brief Initialises a string object.
    /// @param s null terminated string to initialise the data to, may be null for an empty string.
    /// @return A string structure with the correct data, may be allocated unless specified.
    /// @warning May throw `c_throw(bad_alloc)` if the string cannot be allocated.
    /// @note Calling `init(null)` garuntees that the string is not allocated.
    string_init_fun init;

    /// @brief Destroys a string object.
    /// @param str nonnull pointer to the string to be destroyed.
    string_destroy_fun destroy;


    /// @brief Updates the string's capacity to at least `n` characters.
    /// @param str Pointer to the string.
    /// @param n The number of characters the capacity must hold.
    /// @note This function has no affect on the contents of the string.
    /// @exception Throws bad_alloc on failed allocation.
    string_reserve_fun reserve;

    /// @brief Appends a copy of the string formed by the null-terminated character sequence (C-string) pointed by `s`.
    /// @param str Pointer to the string to be modified.
    /// @param s Pointer to a null-terminated character sequence (C-string) to be appended to `str`.
    string_append_fun append;


    /// @brief Changes the length of the string to 0 - erasing the contents of the string.
    /// @param str Pointer to string object.
    string_clear_fun clear;

    /// @brief Resizes the string to a length of n characters.
    /// @param str Pointer to the string to resize.
    /// @param n Number of characters to resize to.
    /// @note - If `n` is smaller than the length of the string, it is shortened to n characters.
    /// @note - If n is greater than the current string length, the current content is extended
    /// @note   by inserting at the end as many null characters as needed to reach a size of n.
    string_resize_fun resize;

    /// @brief Reduces the capacity of the string to its length.
    /// @param str Pointer to the string to be modified.
    string_shrink_to_fit_fun shrink_to_fit;

    /// @brief Returns a newly constructed string object with its value initialized to a copy of a substring of this object.
    /// @param str Pointer to string.
    /// @param pos Position of the first character to be copied as a substring.
    /// @param len Length of the substring - the substring will be from string[pos] to string[pos + len].
    /// @note A value of npos for `len` indicates all characters until the end of the string.
    /// @return A string object with a substring of this object.
    string_substr_fun substr;    

    /// @brief Copies the string into a newly constructed string.
    /// @param str Pointer to string to be copied.
    /// @return A newly constructed string object containing a copy of `str`.
    /// @note The capacity of the newly constructed string is not guarunteed to be the same as `str`.
    string_copy_fun copy;

    /// @brief Inserts additional characters into the string right before the character indicated by `pos`.
    /// @param str Pointer to string object to insert into.
    /// @param pos Location to be inserted into.
    /// @param s A null-terminated C-string to be inserted.
    string_insert_fun insert;

    /// @brief Appends character `c` to the end of the string, increasing its length by one.
    /// @param str Pointer to string to be modified.
    /// @param c Character added to the string.
    string_push_back_fun push_back;

    /// @brief Gets string data.
    /// @param str Pointer to string.
    /// @return A pointer to a null-terminated C-string representing the current value of the string object.
    /// @return - This array includes the same sequence of characters that make up the value of the string
    /// @return   object plus an additional terminating null-character ('\0') at the end.
    /// @return - The pointer returned points to the internal array currently used by the string object to
    /// @return   store the characters that conform its value.
    string_data_fun data;

    /// @brief Gets C-string equivalent.
    /// @param str Pointer to string.
    /// @return A constant pointer to a null-terminated C-string representing the current value of the string object.
    /// @return - The pointer returned points to the internal array currently used by the string object to store the
    /// @return   characters that conform its value.
    string_c_str_fun c_str;

    /// @brief Tests if string is empty.
    /// @param str Pointer to string.
    /// @return Returns whether the string is empty (i.e. whether its length is 0).
    /// @note This function does not modify the value of the string in any way.
    /// @note To clear the content of a string, see clear.
    string_empty_fun empty;

    /// @brief Inserts a copy of src into dest right before pos.
    /// @param dest Pointer to string to be inserted into.
    /// @param pos Position of the first character in str that is inserted into the object as a substring.
    /// @param src Pointer to string object whose data gets inserted as a substring into dest.
    string_inserts_fun inserts;

    bool (*compare)(
        _In_    string                  __notnullptr(str1),
        _In_    const string_value_type __notnullptr(str2))
        __notnull(1) __notnull(2);

    bool (*compares)(
        _In_    string                  __notnullptr(str1),
        _In_    string                  __notnullptr(str2))
        __notnull(1) __notnull(2);
    
    string_size_type (*find)(
        _In_    string                  __notnullptr(str),
        _In_    const string_value_type __notnullptr(s))
        __notnull(1) __notnull(2);
    
    string_value_type (*at)(
        _In_    string                  __notnullptr(str),
        _In_    string_size_type        pos)
        __notnull(1);
} $String;

extern const $String String;

//
// Overload for printing string structures.
//

print_htype
__notnull(1) __notnull(2)
#ifndef __cplusplus
print_pstring(
#else
fprint(
#endif
    FILE *file,
    string __notnullptr(str))
{
    for (
        string_iterator
            it = String.begin(str);
        it < String.end(str); it++)
    {
        putc(*it, file);
    }
}


#ifndef __cplusplus

print_htype
__notnull(1)
print_string(
    FILE *file,
    string str)
{ print_pstring(file, &str); }

#undef generic_fprint_impl
#define generic_fprint_impl(f, s) _Generic((s), \
    string*: print_pstring,                     \
    string: print_string,                       \
    default: _generic_default_fprint(s))(f, s)
#endif

#define optimise_strings(file) setvbuf(file, null, _IOFBF, BUFSIZ);


//
// Because '' gives an integer sometimes,
// define a different function for to_string(char)
// so there is no confusion.
//

inline string
cto_string(
    string_value_type c)
{
    // guaranteed to be 1 character,
    // no point in using init.
    // - and faster (less function calls).
#ifndef __cplusplus
    return (string){.capacity=STRING_SHORT_MAX, .length=1, .__short_data_={c}};
#else
    
    // weird ass C++
    string tmp;

    tmp.length = 1;
    tmp.capacity=STRING_SHORT_MAX;
    tmp.__short_data_[0] = c;

    return tmp;
#endif
}


//
// to_string overloads
//

string _f32_to_string(f32 f);
string _f64_to_string(f64 f);
string _fmax_to_string(fmax_t f);

string _u16_to_string(u16 n);
string _u32_to_string(u32 n);
string _u64_to_string(u64 n);

string _i16_to_string(i16 n);
string _i32_to_string(i32 n);
string _i64_to_string(i64 n);


#ifdef __cplusplus
}
#endif


#ifndef __cplusplus
/// @brief Converts a numeric value to a string.
/// @param x a numeric value to convert.
/// @return An unallocated string holding the converted value.
/// @note The returned string is garunteed to be unallocated,
/// so stuff like `String.append(&s, String.c_str(&to_string(...)))` is valid.
#define to_string(x) _Generic((x),  \
    f32: _f32_to_string,            \
    f64: _f64_to_string,            \
    fmax_t: _fmax_to_string,        \
    u16: _u16_to_string,            \
    u32: _u32_to_string,            \
    u64: _u64_to_string,            \
    i16: _i16_to_string,            \
    i32: _i32_to_string,            \
    i64: _i64_to_string             \
    )(x)
#else

/// @brief Converts a 32 bit float to a string.
/// @param f a numeric value to convert.
/// @return An unallocated string holding the converted value.
/// @note The returned string may be in scientific notation if it exceeds some
//  value.
static inline string
to_string(f32 f)
{ return _f32_to_string(f); }

/// @brief Converts a 64 bit float to a string.
/// @param f a numeric value to convert.
/// @return An unallocated string holding the converted value.
/// @note The returned string may be in scientific notation if it exceeds some
//  value.
static inline string
to_string(f64 f)
{ return _f64_to_string(f); }

/// @brief Converts an fmax_t to a string.
/// @param f a numeric value to convert.
/// @return An unallocated string holding the converted value.
/// @note The returned string may be in scientific notation if it exceeds some
//  value.
static inline string
to_string(fmax_t f)
{ return _fmax_to_string(f); }

/// @brief Converts a 16 bit unsigned integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(u16 n)
{ return _u16_to_string(n); }

/// @brief Converts a 32 bit unsigned integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(u32 n)
{ return _u32_to_string(n); }

/// @brief Converts a 64 bit unsigned integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(u64 n)
{ return _u64_to_string(n); }

/// @brief Converts a 16 bit integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(i16 n)
{ return _i16_to_string(n); }

/// @brief Converts a 32 bit integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(i32 n)
{ return _i32_to_string(n); }

/// @brief Converts a 64 bit integer to a string.
/// @param n a numeric value to convert.
/// @return An unallocated string holding the converted value.
static inline string
to_string(i64 n)
{ return _i64_to_string(n); }


//
// Support for common C++ overloads.
//

inline void operator+=(
    string &self, string_value_type c)
{ String.push_back(&self, c); }

inline void operator+=(
    string &self, const string_value_type *s)
{ String.append(&self, s); }

inline void operator+=(
    string &self, string src)
{ String.append(&self, String.c_str(&src)); }

inline string::string(
    const string_value_type *s)
 : string(String.init(s)) {}

inline string::string(
    const string &rhs)
 : string(String.copy(&(string&)rhs)) {}


//
// Extra print overflow for C++ using references.
//

print_htype
fprint(
    FILE *file,
    string &str)
{
    for (
        string_iterator
            it = String.begin(&str);
        it < String.end(&str); it++)
    {
        putc(*it, file);
    }
}

#endif
