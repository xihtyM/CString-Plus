#include <cstringplus/string.h>

int main(void)
{
    string s = String.init("Hello, World!\n");

    String.end(&s)[-2] = 'E';
    println(&s);

    String.destroy(&s);

    return 0;
}