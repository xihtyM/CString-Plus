#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cstringplus/string.h>


#define test_function_returning_pointer(function, expected_output, arg1, ...) \
    if ((ptr_test = function(arg1, ##__VA_ARGS__)) != expected_output) \
    { \
        printf("Expected function %s to return %p, instead got %p.\n\n", #function, expected_output, ptr_test); \
        failed++; \
    } \
    else \
    { \
        passed++; \
    }

//
// Warning: Do not randomly change the test data.
//

#define UNALLOCATED_DATA    "abcdefg"
#define UNALLOCATED_DATA_2  "hijklmn"


//
// and you ask, why is there no functions, why is it all in the main function...
// and i will tell you...
// yeah i agree but oh well its actually probably more convenient this way at least.
// (cos less passing around variables for testing)
//

int main(void)
{
    void *ptr_test;

    // trackers
    uint32_t failed = 0;
    uint32_t passed = 0;

    string test_unallocated_string = String.init(UNALLOCATED_DATA);

    //
    // Initial test to see if test data is valid.
    //

    if (test_unallocated_string.capacity != STRING_SHORT_MAX)
    {
        // means either it has been allocated or a bug has occurred in the init function.
        perror("Failed initial unallocated string tests");
        printf("ERROR: capacity expected %d but got %d.\n\n", STRING_SHORT_MAX, test_unallocated_string.capacity);

        failed++;
    }
    else
    {
        passed++;
    }

    if (test_unallocated_string.length != strlen(UNALLOCATED_DATA))
    {
        // means that the length of the string was incorrectly set.
        perror("Failed initial unallocated string tests");
        printf("ERROR: length expected %d but got %d.\n\n", strlen(UNALLOCATED_DATA), test_unallocated_string.length);

        failed++;
    }
    else
    {
        passed++;
    }


    for (int i = 0; i < test_unallocated_string.length; i++)
    {
        if (test_unallocated_string.__short_data_[i] != UNALLOCATED_DATA[i])
        {
            failed++;
            perror("Failed initial unallocated string tests");
            printf("ERROR: string data not valid.\n\n");
            break;
        }

        if (i == test_unallocated_string.length - 1) passed++;
    }

    test_function_returning_pointer(String.begin, test_unallocated_string.__short_data_, &test_unallocated_string)
    test_function_returning_pointer(String.end, test_unallocated_string.__short_data_ + strlen(UNALLOCATED_DATA), &test_unallocated_string)

    // test to see if String.data returns a null terminated char array.
    // make sure that the characters ahead are not null before changing with String.data

    *String.end(&test_unallocated_string) = 'h';
    test_function_returning_pointer(String.data, test_unallocated_string.__short_data_, &test_unallocated_string)

    if (*String.end(&test_unallocated_string) != '\x0')
    {
        printf("String.data failed to null-terminate the string.\n\n");
        failed++;
    }
    else
    {
        passed++;
    }

    // still test String.c_str even though it is just a casted String.data call

    *String.end(&test_unallocated_string) = 'h';
    test_function_returning_pointer((string_value_type *)String.c_str, test_unallocated_string.__short_data_, &test_unallocated_string)

    if (*String.end(&test_unallocated_string) != '\x0')
    {
        printf("String.c_str failed to null-terminate the string.\n\n");
        failed++;
    }
    else
    {
        passed++;
    }

    // test String.compare before testing appending
    if (!String.compare(&test_unallocated_string, UNALLOCATED_DATA)
      || String.compare(&test_unallocated_string, "definitely not correct data..."))
    {
        printf("String.compare test failed.\n\n");
        failed++;
    }
    else
    {
        passed++;
    }

    String.append(&test_unallocated_string, UNALLOCATED_DATA_2);

    if (!String.compare(&test_unallocated_string, UNALLOCATED_DATA UNALLOCATED_DATA_2))
    {
        printf("String.append test failed.\n\n");
        failed++;
    }
    else
    {
        passed++;
    }

    // todo: finish this lpeaaaaaaaaaaase

    printf("Passed tests: %d, Failed tests: %d\n", passed, failed);
}