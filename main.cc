#include "include/string.h"

#define LONG_TESTS "�	" \
    "" \
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

int main(void)
{
    string i = String.init(LONG_TESTS);

    for (int it = 0; it < i.length - 5; it++)
    {
        string test_string = String.substr(&i, it, 6);
        print((bool) (String.find(&i, String.c_str(&test_string)) == it));
        putc(' ', stdout);
        println(String.find(&i, String.c_str(&test_string)));
        String.destroy(&test_string);
    }

    println(String.find(&i, String.c_str(&i) + 40));

    String.destroy(&i);

    return 0;
}
