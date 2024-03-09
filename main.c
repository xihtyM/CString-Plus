#include "include/string.h"

#define LONG_TESTS "�	" \
    "" \
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

int main(void)
{
    string i = String.init(LONG_TESTS);

    println(&i);

    return 0;
}
