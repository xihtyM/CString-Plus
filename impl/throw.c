#include "../include/throw.h"
#include "../include/print.h"

void default_bad_alloc_handler(
    u64 size)
{
    fflush(stdout);
    fflush(stderr);

    fprint(stderr, "bad_alloc: size=");
    fprintln(stderr, size);

    exit(1);
}

void default_out_of_range_handler(
    out_of_range_t st)
{
    fflush(stdout);
    fflush(stderr);

    fprint(stderr, "out_of_range: attempted to access pos ");
    fprint(stderr, st.attempted_access);
    fprint(stderr, " when size is ");
    fprintln(stderr, st.size);

    exit(1);
}


void c_throw(
    exception exc)
{
    switch (exc.err)
    {
        case bad_alloc:
        {
            __bad_alloc_handler(exc.size);
            break;
        }
        case out_of_range:
        {
            __out_of_range_handler(exc.out_of_range_struct);
            break;
        }
    }
}
