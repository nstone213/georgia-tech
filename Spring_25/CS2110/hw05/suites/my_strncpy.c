#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

// This is an autograder helper. Do not base your solution off this function!
void h_fill_string(char *dest, size_t dest_sz) {
    // Fill buffer with non-zero value
    memset(dest, 0x69, dest_sz);
}

TestSuite(test_my_strncpy, .timeout = UNREASONABLY_LONG);

Test(test_my_strncpy, basic_1) {
    // Arguments:
    const char *string = "12345";
    size_t n = 2;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, basic_2) {
    // Arguments:
    const char *string = "12345";
    size_t n = 5;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, basic_3) {
    // Arguments:
    const char *string = "12345";
    size_t n = 6;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, basic_4) {
    // Arguments:
    const char *string = "12345";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, basic_5) {
    // Arguments:
    const char *string = "12345";
    size_t n = 0;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, with_inner_null_1) {
    // Arguments:
    const char *string = "12\0 345";
    size_t n = 5;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}

Test(test_my_strncpy, empty) {
    // Arguments:
    const char *string = "";
    size_t n = 5;
    
    // Initialize strings:
    char expected_str[16];
    char actual_str[16];
    h_fill_string(expected_str, sizeof(expected_str));
    h_fill_string(actual_str, sizeof(actual_str));

    // Test `my_strncpy`:
    strncpy(expected_str, string, n);
    char *result = my_strncpy(actual_str, string, n);
    cr_assert(eq(ptr, result, actual_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = actual_str, .size = sizeof(actual_str)};

    cr_assert(eq(mem, actual, expected), "Expected copy to be correct.");
}