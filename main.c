#include <cstringplus/string.h>

void my_bad_alloc_handler(u64 size)
{
    print("You tried to allocate: ");
    print(size);
    print(" bytes.\n");

    println("How silly of you.\n");
}

int main(void)
{
    // Change from default bad_alloc handler to this implementation.
    set_bad_alloc_handler(&my_bad_alloc_handler);

    // This means it is possible to use to_string like this:
    string s = String.init(null);

    // Raises a bad_alloc.
    c_throw((exception){.err=bad_alloc, .size=999999});

    // Now the program will continue.
    println("ahhhh its still running... WHYY!!");

    return 0;
}