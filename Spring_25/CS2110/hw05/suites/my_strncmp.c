#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

// This is an autograder helper. Do not base your solution off this function!
int h_normalize_cmp(int result) {
    if (result > 0)
        return 1;
    if (result < 0)
        return -1;
    return 0;
}

TestSuite(test_my_strncmp, .timeout = UNREASONABLY_LONG);

Test(test_my_strncmp, basic_1) {
    const char *string = "12345";
    const char *string2 = "12345";
    size_t n = 5;

    int expected = h_normalize_cmp(strncmp(string, string2, n));
    int actual = h_normalize_cmp(my_strncmp(string, string2, n));

    cr_assert(eq(int, actual, expected), "Expected comparison to be correct.");
}

Test(test_my_strncmp, basic_2) {
    const char *string = "apples";
    const char *string2 = "oranges";
    size_t n = 8;

    int expected = h_normalize_cmp(strncmp(string, string2, n));
    int actual = h_normalize_cmp(my_strncmp(string, string2, n));

    cr_assert(eq(int, actual, expected), "Expected comparison to be correct.");
}

Test(test_my_strncmp, basic_3) {
    const char *string = "oxygen";
    const char *string2 = "hydrogen";
    size_t n = 10;

    int expected = h_normalize_cmp(strncmp(string, string2, n));
    int actual = h_normalize_cmp(my_strncmp(string, string2, n));

    cr_assert(eq(int, actual, expected), "Expected comparison to be correct.");
}

Test(test_my_strncmp, basic_4) {
    const char *string = "aardvark";
    const char *string2 = "aargh!";

    // Test with n = 3 (should be 0)
    int expected_3 = h_normalize_cmp(strncmp(string, string2, 3));
    int actual_3 = h_normalize_cmp(my_strncmp(string, string2, 3));

    cr_assert(eq(int, actual_3, expected_3), "Expected comparison to be correct.");

    // Test with n = 4 (should be -1)
    int expected_4 = h_normalize_cmp(strncmp(string, string2, 4));
    int actual_4 = h_normalize_cmp(my_strncmp(string, string2, 4));

    cr_assert(eq(int, actual_4, expected_4), "Expected comparison to be correct.");
}

Test(test_my_strncmp, basic_5) {
    const char *string = "apples";
    const char *string2 = "oranges";

    // Test with n = 0 (should be 0)
    int expected_0 = h_normalize_cmp(strncmp(string, string2, 0));
    int actual_0 = h_normalize_cmp(my_strncmp(string, string2, 0));
    cr_assert(eq(int, actual_0, expected_0), "Expected comparison to be correct.");

    // Test with n = 1 (should be -1)
    int expected_1 = h_normalize_cmp(strncmp(string, string2, 1));
    int actual_1 = h_normalize_cmp(my_strncmp(string, string2, 1));
    cr_assert(eq(int, actual_1, expected_1), "Expected comparison to be correct.");
}

Test(test_my_strncmp, with_inner_null_1) {
    const char *string = "12\0 abc";
    const char *string2 = "12\0 def";
    size_t n = 3;

    int expected = h_normalize_cmp(strncmp(string, string2, n));
    int actual = h_normalize_cmp(my_strncmp(string, string2, n));

    cr_assert(eq(int, actual, expected), "Expected comparison to be correct.");
}

Test(test_my_strncmp, with_inner_null_2) {
    const char *string = "12\0 abc";
    const char *string2 = "12\0 def";
    size_t n = 5;

    int expected = h_normalize_cmp(strncmp(string, string2, n));
    int actual = h_normalize_cmp(my_strncmp(string, string2, n));

    cr_assert(eq(int, actual, expected), "Expected comparison to be correct.");
}

Test(test_my_strncmp, subset) {
    const char *string = "abcdef";
    const char *string2 = "abc";

    // Test with n = 3 (should be 0)
    int expected_3 = h_normalize_cmp(strncmp(string, string2, 3));
    int actual_3 = h_normalize_cmp(my_strncmp(string, string2, 3));
    cr_assert(eq(int, actual_3, expected_3), "Expected comparison to be correct.");

    // Test with n = 5 (should be 1)
    int expected_5 = h_normalize_cmp(strncmp(string, string2, 5));
    int actual_5 = h_normalize_cmp(my_strncmp(string, string2, 5));
    cr_assert(eq(int, actual_5, expected_5), "Expected comparison to be correct.");
}