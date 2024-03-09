#include "../include/string.h"
#include <iostream>
#include <assert.h>
#include <string.h>

#define TEST_STRING     "the quick brown fox "
#define TEST_STRING_1   "the quick brown fox jumped over the lazy dog! \\(^)o(^)"
#define TEST_STRING_2   "the quick brown fox jumped over the lazy dog! \\(^)o(^)/"
#define TEST_STRING_3   't'
#define TEST_STRING_4   '/'
#define TEST_STRING_5   TEST_STRING_2

#define TEST_STRING_6   "the quick brown fox jumped over the lazy dog"

int main(void)
{
    string test_string = String.init(null);
    string test_string2 = String.init(TEST_STRING);

    assert(String.empty(&test_string));

    test_string = String.init(TEST_STRING);

    assert(String.compares(&test_string, &test_string2));
    assert(!String.empty(&test_string));
    assert(String.compare(&test_string, TEST_STRING));

    test_string += "jumped over the lazy dog! \\(^)o(^)";
    assert(String.compare(&test_string, TEST_STRING_1));

    test_string += '/';
    assert(String.compare(&test_string, TEST_STRING_2));

    assert(String.at(&test_string, 0) == TEST_STRING_3);
    assert(String.at(&test_string, test_string.length - 1) == TEST_STRING_4);

    assert(strcmp(String.c_str(&test_string), TEST_STRING_5) == 0);

    assert(String.find(&test_string, "brown") == 10);

    String.clear(&test_string);
    assert(test_string.length == 0 && String.empty(&test_string));

    String.destroy(&test_string);

    test_string = String.init(TEST_STRING_6);

    String.destroy(&test_string);
    String.destroy(&test_string2);

    return 0;
}
