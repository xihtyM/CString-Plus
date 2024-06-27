#include "../include/string.h"
#include "../include/to_chars.h"

#if defined(__GNUG__) || defined(__GNUC__)

static inline
u8 bit_length(
    u64 n)
{
    if (!n) return 1;
    return 64 - __builtin_clzll(n);
}

#else

static inline
u8 bit_length(
    u64 n)
{
    if (!n) return 1;

    u8 length = 0;
    while (n > 0) {
        n >>= 1;
        length++;
    }

    return length;
}

#endif


static __constexpr_function
string_iterator string_begin(
    string __notnullptr(str))
{ return str->capacity > STRING_SHORT_MAX
    ? str->__long_data_ : str->__short_data_; }


static __constexpr_function
string_iterator string_end(
    string __notnullptr(str)) { return string_begin(str) + str->length; }


//
// Bugfix note:
// In some gcc compilers, __notnullptr(s)
// generates the following warning:
//   '__builtin_memcpy' reading 25 or more
//   bytes from a region of size 1 [-Wstringop-overread]
//
// - Note, 25 is STRING_SHORT_MAX + 1 currently.
//
// Instead, use *s so that this warning is
// ignored as it does not affect anything.
//
//
// UPDATE:
// - Problem is fixed, in C String.init can now
//   take null as an argument which constructs
//   an empty string instead.
//
// - Note: For C++ passing null also works, however using
//         default constructor is more viable.
//

static string string_init(
    const string_value_type *s)
{
    string str;

    if (!s)
    {
        str.capacity = STRING_SHORT_MAX;
        str.length = 0;
        return str;
    }

    const string_value_type *save = s;
    for (; *save; save++) {}
    str.length = (u64)(save - s);

    if (str.length <= STRING_SHORT_MAX)
    {
        str.capacity = STRING_SHORT_MAX;
        for (string_size_type i = 0; i < str.length; i++)
        {
            str.__short_data_[i] = s[i];
        }
        return str; 
    }

    str.capacity = 1LLU << bit_length(str.length);

#ifndef __cplusplus
    str.__long_data_ = malloc(
        sizeof(string_value_type) * str.capacity);
    
    if (!str.__long_data_)
    {
        c_throw((exception){
            .err=bad_alloc,
            .size=str.capacity
        });

        return String.init(null);
    }
#else
    str.__long_data_ = new string_value_type[str.capacity];
#endif


    for (string_size_type i = 0; i < str.length; i++)
    {
        str.__long_data_[i] = s[i];
    }

    return str;
}


static void string_destroy(
    string __notnullptr(str))
{
    if (str->capacity > STRING_SHORT_MAX
     && str->__long_data_)
    {
#ifndef __cplusplus
        free(str->__long_data_);
#else
        delete[] str->__long_data_;
#endif

        str->__long_data_ = null;
    }
}


static void string_reserve(
    string __notnullptr(str),
    string_size_type size)
{
    if (size <= str->capacity)
        return;

    u64 new_capacity = 1LLU << bit_length(size);

    if (str->capacity <= STRING_SHORT_MAX)
    {
#ifndef __cplusplus
        string_value_type *new_data = malloc(
            sizeof(string_value_type) * new_capacity);
        
        if (!new_data)
        {
            c_throw((exception){
                .err=bad_alloc,
                .size=new_capacity
            });
            return;
        }
#else
        string_value_type *new_data = new string_value_type[new_capacity];
#endif


        str->capacity = new_capacity;

        for (string_size_type i = 0; i < str->length; i++)
        {
            new_data[i] = str->__short_data_[i];
        }

        str->__long_data_ = new_data;
        return;
    }

#ifndef __cplusplus
    str->__long_data_ = realloc(
        str->__long_data_, new_capacity);
    
    if (!str->__long_data_)
    {
        c_throw((exception){
            .err=bad_alloc,
            .size=str->capacity
        });
        return;
    }
#else
    string_value_type *new_data = new string_value_type[new_capacity];
    for (string_size_type i = 0; i < str->length; i++)
    {
        new_data[i] = str->__long_data_[i];
    }

    delete[] str->__long_data_;
    str->__long_data_ = new_data;
#endif

    str->capacity = new_capacity;
}


static void
string_append(
    string __notnullptr(str),
    const string_value_type __notnullptr(s))
{
    // get the length of s
    const string_value_type *save = s;
    for (; *save; save++) {}
    u64 s_len = (u64)(save - s);
    str->length += s_len;

    if (str->capacity < str->length)
        string_reserve(str, str->length);

    //
    // Copy contents of s into
    // string str.
    //

    string_iterator end = string_end(str) - s_len;

    for (string_size_type i = 0; i < s_len; i++)
    {
        end[i] = s[i];
    }
}


static inline void
string_clear(
    string __notnullptr(str)) { str->length = 0; }


static void
string_resize(
    string __notnullptr(str),
    string_size_type len)
{
    if (len > str->capacity)
        string_reserve(str, len);

    if (len > str->length)
    {
        string_iterator begin = string_begin(str);

        for (string_iterator it = begin + str->length
           ; it < (begin + len); it++)
        {
            *it = '\x00';
        }
    }

    str->length = len;
}


static void
string_shrink_to_fit(
    string __notnullptr(str))
{
    if (str->capacity <= STRING_SHORT_MAX)
    {
        return;
    }

    if (str->length <= STRING_SHORT_MAX)
    {
        str->capacity = STRING_SHORT_MAX;

        string_iterator save = str->__long_data_;
        for (string_size_type i = 0; i < str->length; i++)
        {
            str->__short_data_[i] = save[i];
        }

#ifndef __cplusplus
        free(save);
#else
        delete[] save;
#endif
        return;
    }

    str->capacity = str->length;

#ifndef __cplusplus
    str->__long_data_ = realloc(
        str->__long_data_, str->capacity);
    
    if (!str->__long_data_)
    {
        c_throw((exception){
            .err=bad_alloc,
            .size=str->capacity
        });
        return;
    }
#else
    string_value_type *new_data = new string_value_type[str->capacity];
    for (string_size_type i = 0; i < str->length; i++)
    {
        new_data[i] = str->__long_data_[i];
    }

    delete[] str->__long_data_;
    str->__long_data_ = new_data;
#endif
}


static string
string_substr(
    string __notnullptr(str),
    string_size_type pos,
    string_size_type len)
{
    if ((len + pos) > str->length)
    {
        c_throw((exception){
            .err=out_of_range,
            .out_of_range_struct=(out_of_range_t){
                .attempted_access=len+pos,
                .size=str->length
            }
        });
        return String.init(null);
    }

    string new_string;
    new_string.length = len;

    string_iterator start = string_begin(str) + pos;

    if (len <= STRING_SHORT_MAX)
    {
        new_string.capacity = STRING_SHORT_MAX;

        for (string_size_type i = 0; i < len; i++)
        {
            new_string.__short_data_[i] = start[i];
        }

        return new_string;
    }

    new_string.capacity = 1LLU << bit_length(len);

#ifndef __cplusplus
    new_string.__long_data_ = malloc(
        sizeof(string_value_type) * new_string.capacity);
    
    if (!new_string.__long_data_)
    {
        c_throw((exception){
            .err=bad_alloc,
            .size=new_string.capacity
        });
        return String.init(null);
    }
#else
    new_string.__long_data_ = new string_value_type[new_string.capacity];
#endif


    for (string_size_type i = 0; i < len; i++)
    {
        new_string.__long_data_[i] = start[i];
    }

    return new_string;
}


static string
string_copy(
    string __notnullptr(str))
{
    string copy;
    copy.length = str->length;
    string_iterator start = string_begin(str);

    if (copy.length <= STRING_SHORT_MAX)
    {
        copy.capacity = STRING_SHORT_MAX;
        for (string_size_type i = 0; i < copy.length; i++)
        {
            copy.__short_data_[i] = start[i];
        }

        return copy;
    }

    copy.capacity = 1ULL << bit_length(copy.length);
#ifndef __cplusplus
    copy.__long_data_ = malloc(
        sizeof(string_value_type) * copy.capacity);
    
    if (!copy.__long_data_)
    {
        c_throw((exception){
            .err=bad_alloc,
            .size=copy.capacity
        });
        return String.init(null);
    }
#else
    copy.__long_data_ = new string_value_type[copy.capacity];
#endif


    for (string_size_type i = 0; i < copy.length; i++)
    {
        copy.__long_data_[i] = start[i];
    }

    return copy;
}


static inline
string_value_type *
string_data(
    string __notnullptr(str))
{
    if (str->capacity <= str->length)
        string_reserve(str, str->length + 1);

    // make sure string is null terminated
    *string_end(str) = '\x00';

    return string_begin(str);
}


static inline const
string_value_type *
string_c_str(
    string __notnullptr(str)) { return (const string_value_type *)string_data(str); }


static void
string_insert(
    string __notnullptr(str),
    string_size_type pos,
    const string_value_type __notnullptr(s))
{
    const string_value_type *save = s;
    for (; *save; save++) {}
    u64 s_len = (u64)(save - s);

    u64 new_length = str->length + s_len;

    if (new_length > str->capacity)
        string_reserve(str, new_length);

    string_iterator back = string_end(str) - 1;
    for (; back >= string_begin(str) + pos; back--)
    {
        back[s_len] = *back;
    }

    for (string_size_type i = 0; i < s_len; i++)
    {
        back[i + 1] = s[i];
    }

    str->length = new_length;
}


static inline void
string_inserts(
    string __notnullptr(dest),
    string_size_type pos,
    string __notnullptr(src))
{
    string_insert(dest, pos, string_c_str(src));
}


static void
string_push_back(
    string __notnullptr(str),
    string_value_type c)
{
    if (str->length >= str->capacity)
        string_reserve(str, str->length + 1);

    *string_end(str) = c;
    str->length++;
}


static __constexpr_function bool
string_empty(
    string __notnullptr(str)) { return str->length == 0; }


static inline bool
string_compare(
    string __notnullptr(str),
    const string_value_type __notnullptr(s))
{
    string_iterator start = string_begin(str);

    //
    // Note: Cannot compare the start (address) of this string (str)
    // to the address of the C-style string (s).
    //
    // This is because one is null-terminated whereas this implementation
    // is terminated by length.
    //
    // Comparing pointers could lead to weird situations where both strings
    // contain a null character and are different but start at the same place in memory.
    // 
    // - Causing undefined behaivour. Therefore, do not compare addresses.
    //

    for (string_size_type i = 0; start + i < string_end(str); i++)
    {
        if (start[i] != s[i] || !s[i])
            return false;
    }

    return true;
}


static inline bool
string_compares(
    string __notnullptr(str1),
    string __notnullptr(str2))
{
    // If lengths are not equal
    // they cannot be the same.
    if (str1->length != str2->length)
        return false;


    //
    // Compares the pointers, if both
    // str1 and str2 point to the same
    // object then no need to compare
    // as they are the same string.
    //

    if (str1 == str2)
        return true;

    return string_compare(
        str1, string_c_str(str2));
}


static inline
string_value_type string_at(
    string __notnullptr(str),
    string_size_type pos)
{
    if (pos > str->length)
    {
        c_throw((exception){
            .err=out_of_range,
            .out_of_range_struct=(out_of_range_t){
                .attempted_access=pos,
                .size=str->length
            }
        });

        return 0;
    }

    return string_begin(str)[pos];
}


static string_size_type
__string_knuth_morris_pratt_search(
    string __notnullptr(str),
    const string_value_type __notnullptr(pattern),
    const string_size_type pattern_len)
{
    //
    // Pretty much copied this algorithm so I don't really understand it...
    // But here is where I found it for future:
    // - https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
    //

    const string_size_type str_len = str->length;

    string_size_type failure[pattern_len];

    failure[0] = 0;

    for (string_size_type i = 1, len = 0; i < pattern_len; )
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            failure[i] = len;
            i++;
        }
        else
        {
            if (len)
            {
                len = failure[len - 1];
            }
            else
            {
                failure[i] = 0;
                i++;
            }
        }
    }

    for (string_size_type i = 0, j = 0; i < str_len; )
    {
        if (pattern[j] == string_at(str, i))
        {
            j++;
            i++;
        }
        
        if (j == pattern_len)
        {
            return i - j;
        }
        else if (i < str_len && pattern[j] != string_at(str, i))
        {
            if (j)
            {
                j = failure[j - 1];
            }
            else
            {
                i++;
            }
        }
    }

    return npos;
}

static inline
string_size_type
string_find(
    string __notnullptr(str),
    const string_value_type __notnullptr(s))
{
    // 
    // return npos if the string doesn't contain anything
    // return npos if the string is too small to fit s
    // otherwise use kmp search to try and find the string.
    //
    
    if (str->length <= 0)
        return npos;
    
    const_string_iterator save = s;
    for (; *save; save++);
    const string_size_type length = save - s;

    if (length > str->length || !length)
        return npos;

    return __string_knuth_morris_pratt_search(str, s, length);
}


__constexpr
struct $String
String = {
    string_begin,
    string_end,
    string_init,
    string_destroy,
    string_reserve,
    string_append,
    string_clear,
    string_resize,
    string_shrink_to_fit,
    string_substr,
    string_copy,
    string_insert,
    string_push_back,
    string_data,
    string_c_str,
    string_empty,
    string_inserts,
    string_compare,
    string_compares,
    string_find,
    string_at,
};


#ifdef __cplusplus
extern "C"
{
#endif


//
// tostring conversions
//


string
_u16_to_string(
    u16 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}

string
_u32_to_string(
    u32 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}

string
_u64_to_string(
    u64 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}


string
_i16_to_string(
    i16 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}

string
_i32_to_string(
    i32 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}

string
_i64_to_string(
    i64 n)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        n);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}


string
_f32_to_string(
    f32 f)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        f);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}

string
_f64_to_string(
    f64 f)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        f);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}


string
_fmax_to_string(
    fmax_t f)
{
    string str = String.init(null);
    string_iterator begin = string_begin(&str);

    to_chars_result res = to_chars(
        begin,
        begin + str.capacity,
        f);

    if (res.errc != success)
    {
        return str;
    }

    str.length = res.ptr - begin;

    return str;
}


#ifdef __cplusplus
}
#endif
