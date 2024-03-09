#include "../include/print.h"
#include "../include/to_chars.h"

#ifdef __cplusplus
extern "C" {
#endif

print_type print_x16(FILE *file, u16 n)
{
    if (!n)
    {
        putc('0', file);
        return;
    }

    // small optimisation for smaller numbers
    if (n < 10)
    {
        putc('0' + n, file);
        return;
    }
    
    // no null terminator
    char digits[4];
    u8 len = 0;

    while (n >= 10)
    {
        digits[len] = '0' + (n % 10);
        n /= 10;
        len++;
    }

    putc('0' + n, file);

    for (; len > 0; len--)
    {
        putc(digits[len - 1], file);
    }
}

print_type print_x32(FILE *file, u32 n)
{
    if (!n)
    {
        putc('0', file);
        return;
    }

    // small optimisation for smaller numbers
    if (n < 10)
    {
        putc('0' + n, file);
        return;
    }
    
    // no null terminator
    char digits[10];
    u8 len = 0;

    while (n >= 10)
    {
        digits[len] = '0' + (n % 10);
        n /= 10;
        len++;
    }

    putc('0' + n, file);

    for (; len > 0; len--)
    {
        putc(digits[len - 1], file);
    }
}

print_type print_x64(FILE *file, u64 n)
{
    if (!n)
    {
        putc('0', file);
        return;
    }

    // small optimisation for smaller numbers
    if (n < 10)
    {
        putc('0' + n, file);
        return;
    }
    
    // no null terminator
    char digits[19];
    u8 len = 0;

    while (n >= 10)
    {
        digits[len] = '0' + (n % 10);
        n /= 10;
        len++;
    }

    putc('0' + n, file);

    for (; len > 0; len--)
    {
        putc(digits[len - 1], file);
    }
}


print_type
print_f32(FILE *file, f32 f)
{
    char buf[24];

    to_chars_result res = to_chars(buf, buf + 22, f);

    if (!res.ptr || res.errc != success)
    {
        print("Error code: ");
        println(res.errc);
        return;
    }

    buf[24] = '\0';

    print(buf);
}

print_type
print_f64(FILE *file, f64 f)
{
    char buf[36];

    to_chars_result res = to_chars(buf, buf + 34, f);

    if (!res.ptr || res.errc != success)
    {
        print("Error code: ");
        println(res.errc);
        return;
    }

    *res.ptr = '\0';

    print(buf);
}

print_type
print_fmax(FILE *file, fmax_t f)
{
    char buf[36];

    to_chars_result res = to_chars(buf, buf + 34, f);

    if (!res.ptr || res.errc != success)
    {
        print("Error code: ");
        println(res.errc);
        return;
    }

    *res.ptr = '\0';

    print(buf);
}



#ifdef __cplusplus
}
#endif
