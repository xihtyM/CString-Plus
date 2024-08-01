#pragma once

#ifdef NON_NULL
#undef NON_NULL
#endif

#ifdef NON_NULL_PTR
#undef NON_NULL_PTR
#endif

#ifdef null_t
#undef null_t
#endif

#ifdef null
#undef null
#endif

#if defined(__GNUC__) || defined(__clang__)

#pragma GCC diagnostic error "-Wnonnull"

#ifdef __MINGW_ATTRIB_NONNULL
#define NON_NULL(arg) __MINGW_ATTRIB_NONNULL(arg)
#else
#define NON_NULL(arg) __attribute__((__nonnull__(arg)))
#endif

#else
#define NON_NULL(arg)
#endif

#ifdef __cplusplus

#define NON_NULL_PTR(lvalue) *lvalue

#if __cplusplus >= 201103L

//
// C++11 introduces nullptr
// use this as null because
// it is the best to use.
//

#define null nullptr
typedef decltype(null) null_t;

#else

//
// Before C++11 you can still
// define your own class for
// nullptr to be emulated.
//

// for std::ostream
#include <ostream>

// NOTE: the address of null
// is not garunteed to be 0
// with this implementation
const class null_t
{
public:
    template<typename T>
    operator T() const
    {
        return (T) 0;
    }

    friend std::ostream& operator<<(std::ostream &os, const null_t &n) {
        os << "null";
        return os;
    }
private:
    void operator&() const; // illegal
    void operator*() const; // illegal
} null = {};

// end #if __cplusplus >= 201103L
#endif

// end #ifdef __cplusplus
// #ifndef __cplusplus
#else

#define NON_NULL_PTR(lvalue) lvalue[static 1]

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202000L

//
// c23 declares support for keywords:
// - nullptr
// - typeof
//

#define null nullptr
typedef typeof(null) null_t;

#else

//
// Before c23:
//
// If __GNUG__ is defined, use __null 
// for null and typeof(__null) (defined
// as long long) for null_t.
//
// Otherwise fall back to using void*
// for null_t and ((null_t)0) for null.
//


#ifdef __GNUG__
typedef long long null_t;
#define null __null
#else
typedef void *null_t;
#define null ((null_t)0)
#endif

#endif
#endif
