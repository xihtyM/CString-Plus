#include <cstringplus/string.h>
#include <math.h>


int main(int argc, char *argv[])
{
    optimise_strings(stdout);

    string str = to_string(__FLT_MAX__);
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(__DBL_MAX__))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(__LDBL_MAX__))));
    String.push_back(&str, '\n');

    String.append(&str, String.c_str(to_ptr(to_string(__FLT_MIN__))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(__DBL_MIN__))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(__LDBL_MIN__))));
    String.push_back(&str, '\n');

    String.append(&str, String.c_str(to_ptr(to_string(0.999999e-5f))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(0.999999e-5))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(0.999999e-5l))));
    String.push_back(&str, '\n');

    String.append(&str, String.c_str(to_ptr(to_string(INFINITY))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(-INFINITY))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(NAN))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(-NAN))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(0.f))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(0.))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(0.l))));
    String.push_back(&str, '\n');

    String.append(&str, String.c_str(to_ptr(to_string(-__FLT_MIN__))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(-__DBL_MIN__))));
    String.push_back(&str, '\n');
    String.append(&str, String.c_str(to_ptr(to_string(-__LDBL_MIN__))));
    String.push_back(&str, '\n');

    for (f64 f = 0.1; f < 10.; f += 0.1)
    {
        String.append(&str, String.c_str(to_ptr(to_string(f))));
        String.push_back(&str, '\n');
    }

    for (f64 f = 0.1; f < 10.; f += 0.1)
    {
        String.append(&str, String.c_str(to_ptr(to_string(-f))));
        String.push_back(&str, '\n');
    }

    for (i16 i = -i8_max; i <= i8_max; i++)
    {
        String.append(&str, String.c_str(to_ptr(to_string(i))));
        String.push_back(&str, '\n');
    }


    println(&str);

    print("Total length: ");
    println(str.length);
    print("Capacity: ");
    println(str.capacity);

    String.destroy(&str);

    return 0;
}
