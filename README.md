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
* Out of range and bad alloc throws.

## Installation: ##

### Windows: ###
There isn't a way of automatically installing this library right now; however here is a way to install it with a mingw compiler.

> Note: If you are using a 32-bit machine for some reason, change the 64 to 32.

**Installing the library:**
1. Download the latest release [here](https://github.com/xihtyM/string/releases/download/v1.0/libcstringplus.lib).
2. Go to (Drive Name):\msys64\mingw64\lib (the installation path may be different on your machine; however this is the most common).
3. Paste/move the lib file into this folder.

**Installing the include files:**
1. Download every file inside the [include](https://github.com/xihtyM/string/tree/main/include) directory.
2. Create a folder inside your include directory (name doesn't matter - although keep it relevant to the libraries use for social credit).
3. Paste/move the files into that folder.

## Using the library: ##
> Note: I will be using the folder name of `cstringplus` as that is the name of the library.

Starting with simple construction and destruction:
```c
#include <cstringplus/string.h>

int main(void)
{
    // Creating a string.
    string i = String.init("Hello World!");

    // Printing the string.
    println(&i);

    // Freeing memory.
    String.destroy(&i);

    return 0;
}
```

