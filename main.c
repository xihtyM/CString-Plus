#include "include/string.h"

int main(void)
{
    string i = String.init("Hello World!");

    println(&i);

    String.destroy(&i);

    return 0;
}
