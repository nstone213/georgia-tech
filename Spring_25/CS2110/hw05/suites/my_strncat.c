#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

// andy garcha

// This is an autograder helper. Do not base your solution off this function!
void h_init_string(char *dest, size_t dest_sz, const char *src, size_t src_sz) {
    // Fill buffer with non-zero value
    memset(dest, 0x69, dest_sz);
    memcpy(dest, src, src_sz);
}

TestSuite(test_my_strncat, .timeout = UNREASONABLY_LONG);

Test(test_my_strncat, basic_1) {
    // Arguments:
    const char str_a[] = "12345";
    const char str_b[] = "6789";
    size_t n = 2;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, basic_2) {
    // Arguments:
    const char str_a[] = "12345";
    const char str_b[] = "6789";
    size_t n = 9;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, basic_3) {
    // Arguments:
    const char str_a[] = "12345";
    const char str_b[] = "6789";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, basic_4) {
    // Arguments:
    const char str_a[] = "12345";
    const char str_b[] = "6789";
    size_t n = 15;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, basic_5) {
    // Arguments:
    const char str_a[] = "12345";
    const char str_b[] = "6789";
    size_t n = 0;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, with_inner_null_1) {
    // Arguments:
    const char str_a[] = "he\0athen";
    const char str_b[] = "llo world";
    size_t n = 3;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, with_inner_null_2) {
    // Arguments:
    const char str_a[] = "he\0athen";
    const char str_b[] = "llo world";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, with_inner_null_3) {
    // Arguments:
    const char str_a[] = "hello";
    const char str_b[] = "worm\0ld";
    size_t n = 3;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, with_inner_null_4) {
    // Arguments:
    const char str_a[] = "hello";
    const char str_b[] = "worm\0ld";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, empty_1) {
    // Arguments:
    const char str_a[] = "";
    const char str_b[] = "";
    size_t n = 2;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, empty_2) {
    // Arguments:
    const char str_a[] = "";
    const char str_b[] = "abc";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}

Test(test_my_strncat, empty_3) {
    // Arguments:
    const char str_a[] = "abc";
    const char str_b[] = "";
    size_t n = 10;
    
    // Initialize strings:
    char expected_str[32];
    char student_str[32];
    h_init_string(expected_str, sizeof(expected_str), str_a, sizeof(str_a));
    h_init_string(student_str, sizeof(student_str), str_a, sizeof(str_a));

    // Test `my_strncat`:
    strncat(expected_str, str_b, n);
    char *result = my_strncat(student_str, str_b, n);
    cr_assert(eq(ptr, result, student_str), "Expected return value to match destination pointer.");

    struct cr_mem expected = {.data = expected_str, .size = sizeof(expected_str)};
    struct cr_mem actual = {.data = student_str, .size = sizeof(student_str)};

    cr_assert(eq(mem, actual, expected), "Expected concatenation to be correct.");
}