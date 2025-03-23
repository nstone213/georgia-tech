#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_my_memset, .timeout = UNREASONABLY_LONG);

Test(test_my_memset, basic_1) {
    // Arguments:
    char *original = "i love aardvarks";
    int ch = 'a';
    size_t count = 1;

    // Initialize strings:
    char expected_dest[17];
    char actual_dest[17];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, basic_2) {
    // Arguments:
    char *original = "i love aardvarks";
    int ch = 'a';
    size_t count = 7;

    // Initialize strings:
    char expected_dest[17];
    char actual_dest[17];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, basic_3) {
    // Arguments:
    char *original = "i love aardvarks";
    int ch = 0x9961;
    size_t count = 16;

    // Initialize strings:
    char expected_dest[17];
    char actual_dest[17];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, basic_4) {
    // Arguments:
    char *original = "abcdefghij";
    int ch = '-';
    size_t count = 6;

    // Initialize strings:
    char expected_dest[11];
    char actual_dest[11];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, basic_5) {
    // Arguments:
    char *original = "abcdefghij";
    int ch = 'b';
    size_t count = 7;

    // Initialize strings:
    char expected_dest[11];
    char actual_dest[11];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, with_null_ch) {
    // Arguments:
    char *original = "abcdefghij";
    int ch = '\0';
    size_t count = 6;

    // Initialize strings:
    char expected_dest[11];
    char actual_dest[11];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, with_null_in_dest) {
    // Arguments:
    char *original = "abcd\0\0ghij";
    int ch = 'b';
    size_t count = 8;

    // Initialize strings:
    char expected_dest[11];
    char actual_dest[11];
    memcpy(expected_dest, original, sizeof(expected_dest));
    memcpy(actual_dest, original, sizeof(actual_dest));

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(
        eq(mem,
           ((struct cr_mem){.data = expected_dest, .size = sizeof(expected_dest)}),
           ((struct cr_mem){.data = original, .size = sizeof(expected_dest)})
        ),
        "Expected autograder buffer to stay the same after execution"
    );
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}

Test(test_my_memset, pre_written) {
    // Arguments:
    char *original ="Hello my name is computer and i am testing for this student yeah abcdefghijklmnopqrstuvwxyz now i k";
    int ch = '-';
    size_t count = 6;

    // Initialize strings:
    char expected_dest[100];
    char actual_dest[100];
    strcpy(expected_dest, original);
    strcpy(actual_dest, original);

    // Test `my_memset`:
    char *result = my_memset(actual_dest, ch, count);
    cr_assert(eq(ptr, result, actual_dest), "Expected return value to match destination pointer.");
    cr_assert(eq(str, expected_dest, original), "Expected autograder buffer to stay the same after execution");
    memset(expected_dest, ch, count);

    struct cr_mem expected = {.data = expected_dest, .size = sizeof(expected_dest)};
    struct cr_mem actual = {.data = actual_dest, .size = sizeof(actual_dest)};

    cr_assert(eq(mem, actual, expected), "Expected memory to be correct.");
}