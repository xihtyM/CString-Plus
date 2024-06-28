# String Library for C #

**Warning: Do not use C++ with this library for now - it has limited functionality and the standard library is better.**

## What this library has: ##

This library is a simple string library for C with similar style functions to that of the C++ standard library.
Here is a list of some of the features:
* Constant and non-constant iterators (in the form of pointers).
* String manipulation functions (including insertion, appending, copies, substrings, clearing etc).
* Memory management (strings will still require destruction due to the limitations of C).
* Misc functions (such as comparing strings, finding substrings, conversions to and from C-strings etc).
* Function for printing strings.
* A C++ wrapper (although I'd recommend just using the standard C++ library for a fully C++ project). **May be updated to work better in the future.**
* Exception handling for out of range and bad allocations (further explained [here](#errors))

## Installation: ##

### Windows: ###
There isn't a way of automatically installing this library right now; however here is a way to install it with a mingw compiler.

> Note: If you are using a 32-bit machine for some reason, change the 64 to 32.

**Installing the library:**
1. Download the latest release [here](https://github.com/xihtyM/string/releases/download/v1.1/libcstringplus.lib).
2. Go to (Drive Name):\msys64\mingw64\lib (the installation path may be different on your machine; however this is the most common).
3. Paste/move the lib file into this folder.

**Installing the include files:**
1. Download every file inside the [include](https://github.com/xihtyM/string/tree/main/include) directory.
2. Create a folder inside your include directory (name doesn't matter - although keep it relevant to the libraries use for social credit).
3. Paste/move the files into that folder.

## Using the library: ##
> Note: I will be using the folder name of `cstringplus` as that is the name of the library.

### The library tries to bring C++'s string library to C. ###

C++ code:
```cpp
#include <iostream>
#include <string>

int main(void)
{
    std::string s = "Hello, World!\n";

    for (std::string::iterator it = s.begin(); it < s.end(); it++)
    {
        std::cout << *it;
    }

    return 0;
}
```

Equivalent with cstringplus in C:
```c
#include <cstringplus/string.h>

int main(void)
{
    string s = String.init("Hello World!\n");

    for (string_iterator it = String.begin(&s); it < String.end(&s); it++)
    {
        print(*it);
    }

    String.destroy(&s);

    return 0;
}
```

### Conversions between data types with to_string: ###

`to_string` guarantees an unallocated string, therefore it is possible to use anywhere without fear of having to destroy the object.
> Note: Obviously, modifying this string removes any guarantee of allocation.

```c
#include <cstringplus/string.h>

int main(void)
{
    // This means it is possible to use to_string like this:
    string s = to_string(420.69);
    println(&s);

    // Or like this:
    println(to_string(555.555));

    // Or like this (faster but more verbose):
    println(to_ptr(to_string(44.5)));

    return 0;
}
```

`to_string` has implementations for every single numerical data type there is (decimals, integers and unsigned integers).

### Handling errors: <a id="errors"></a>
This library throws 2 different errors right now, there is no way to "catch" them; however you can change the handler function which gets called for each error.

The 2 errors are `out_of_range` and `bad_alloc`. The default handler function for both of these outputs the error message to the console and then exits the program.
If the function being used throws an error and returns a string, it should **always return an empty string**.

You can change the handler function with your own implementation with the functions included in `throw.h`. A neat example could be:
```c
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

    // Raises a bad_alloc.
    c_throw((exception){.err=bad_alloc, .size=999999});

    // Now the program will continue.
    println("ahhhh its still running... WHYY!!");

    return 0;
}
```

The same is possible for `out_of_range`.
