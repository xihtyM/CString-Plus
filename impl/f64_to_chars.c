#include "../include/to_chars.h"
#include "../include/string.h"


//
// Special cases for converting f64s
//

static to_chars_result
__handle_special_value_f64(
    char *first,
    char *const last,
    const f64 value)
{
    string str = String.init(null);

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    switch (__builtin_expect(fpclassify(value), FP_NORMAL))
#else
    switch (fpclassify(value))
#endif
    {
        case FP_INFINITE:
            String.append(&str, "inf");
            break;
        
        case FP_NAN:
            String.append(&str, "nan");
            break;
        
        case FP_ZERO:
            break;

        default:
        case FP_SUBNORMAL:
        case FP_NORMAL:
            return (to_chars_result){null, success};
    }

    const bool sign = value < 0;

    if (sign)
        *first++ = '-';
    
    if (!String.empty(&str))
    {
        if (last - first < str.length)
            return (to_chars_result){last, out_of_range};
        
        const_string_iterator it = String.c_str(&str);

        for (u8 i = 0; i < str.length; i++)
        {
            first[i] = it[i];
        }

        first += str.length;

        String.destroy(&str);

        return (to_chars_result){first, success};
    }

    if (last <= first)
        return (to_chars_result){last, out_of_range};

    *first++ = '0';

    return (to_chars_result){first, success};
}


static to_chars_result
_f64_to_chars_raw(
    char *first,
    char *const last,
    const f64 value)
{
    // assert(value > u32_max)
    const bool sign = value < 0;
    const f64 abs_value = sign ? -value : value;
    const u64 int_part = abs_value + 5e-6; // if value is 2.99..., round to 3
    const u64 decimal_part = ((u64)(((abs_value - int_part) * (10u * (u32)fmultiplier)) + 5) / 10);

    if (int_part < 10)
    {
        u8 expected_length = sign + 2 + fdigits;

        if (last - first < expected_length)
            return (to_chars_result){last, out_of_range};
        
        if (sign)
            *first++ = '-';
        *first++ = '0' + int_part;
    }
    else
    {
        u8 int_part_length = 0;
        for (u64 i = int_part; i; i /= 10, int_part_length++) {}

        u8 expected_length = sign + int_part_length + 1 + fdigits;

        if (last - first < expected_length)
            return (to_chars_result){last, out_of_range};
        
        if (sign)
            *first++ = '-';

        first += int_part_length;

        u64 n = int_part;
        for (u8 i = 1; n && i <= int_part_length; i++)
        {
            first[-i] = '0' + (n % 10);
            n /= 10;
        }
    }

    *first++ = '.';

    u8 offset = 0;

    if ((10 * decimal_part) < fmultiplier)
    {
        u64 decimal_part_save = decimal_part;

        for (; decimal_part_save; offset++)
        {
            *first++ = '0';
            decimal_part_save /= 10;
        }
    }

    first += fdigits - offset;
    u64 n = decimal_part;

    u8 i;
    for (i = 1; n && i <= fdigits; i++)
    {
        first[-i] = '0' + (n % 10);
        n /= 10;
    }

    while (i <= fdigits)
    {
        first[-i] = '0';
        i++;
    }
    
    return (to_chars_result){first, success};
}


static to_chars_result
__handle_u64overflow_f64(
    char *first,
    char *const last,
    const f64 value)
{
    const f32 abs_value = value < 0 ? -value : value;
    if (abs_value < u32_max && abs_value > 1e-5)
        return (to_chars_result){null, success};
    
    f64 mantissa = value;
    i16 exponent = 0;

    while (mantissa >= 10. || mantissa <= -10.)
    {
        mantissa /= 10.;
        exponent++;
    }

    while (mantissa < 1. && mantissa > -1.)
    {
        mantissa *= 10.;
        exponent--;
    }

    // just in case it gets represented as 10...e...

    if (mantissa > 9.99999 || mantissa < -9.99999)
    {
        exponent += mantissa < 0 ? -1 : 1;
        mantissa = 1.;
    }
    
    const bool exp_signed = exponent < 0;
    const u16 abs_exponent = exp_signed ? -exponent : exponent;
    u8 expected_output_length = 5 + fdigits + (abs_exponent >= 100 ? 2 : abs_exponent >= 10);

    if (last - first < expected_output_length)
        return (to_chars_result){last, out_of_range};
    
    to_chars_result res = _f64_to_chars_raw(first, last, mantissa);
    
    if (!res.ptr || res.errc != success)
        return res;
    
    first = res.ptr;

    *first++ = 'e';    
    *first++ = exp_signed ? '-' : '+';
    
    
    // slightly faster than a while loop

    if (abs_exponent < 10)
    {
        *first++ = '0' + abs_exponent;
    }
    else if (abs_exponent < 100)
    {
        *first++ = '0' + abs_exponent / 10;
        *first++ = '0' + abs_exponent % 10;
    }
    else
    {
        *first++ = '0' + abs_exponent / 100;
        *first++ = '0' + (abs_exponent / 10) % 10;
        *first++ = '0' + abs_exponent % 10;
    }


    return (to_chars_result){first, success};
}

//
// internal to_chars for f64s
//

to_chars_result
_f64_to_chars(
    char *first,
    char *const last,
    const f64 value)
{
    to_chars_result result;

    if ((result = __handle_special_value_f64(first, last, value)).ptr || result.errc != success)
        return result;
    
    if ((result = __handle_u64overflow_f64(first, last, value)).ptr || result.errc != success)
        return result;

    return _f64_to_chars_raw(first, last, value);
}
