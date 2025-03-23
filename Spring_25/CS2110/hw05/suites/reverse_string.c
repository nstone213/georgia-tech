#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

Test(test_reverse_string, basic_1) {
    // Arguments:
    char *original = "Mississippi";

    // Initialize strings:
    char expected_str[12];
    char actual_str[12];
    strcpy(expected_str, original);
    strcpy(actual_str, original);

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(eq(str, expected_str, original), "Expected autograder buffer to stay the same after execution");
    h_reverse_string(expected_str);

    cr_assert(eq(str, actual_str, expected_str), "Expected reversing string to be correct.");
}

Test(test_reverse_string, basic_2) {
    // Arguments:
    char *original = "bookkeeper";

    // Initialize strings:
    char expected_str[11];
    char actual_str[11];
    strcpy(expected_str, original);
    strcpy(actual_str, original);

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(eq(str, expected_str, original), "Expected autograder buffer to stay the same after execution");
    h_reverse_string(expected_str);

    cr_assert(eq(str, actual_str, expected_str), "Expected reversing string to be correct.");
}

Test(test_reverse_string, basic_3) {
    // Arguments:
    char *original = "Zzyzx";

    // Initialize strings:
    char expected_str[6];
    char actual_str[6];
    strcpy(expected_str, original);
    strcpy(actual_str, original);

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(eq(str, expected_str, original), "Expected autograder buffer to stay the same after execution");
    h_reverse_string(expected_str);

    cr_assert(eq(str, actual_str, expected_str), "Expected reversing string to be correct.");
}

Test(test_reverse_string, basic_4) {
    // Arguments:
    char *original = "12345";

    // Initialize strings:
    char expected_str[6];
    char actual_str[6];
    strcpy(expected_str, original);
    strcpy(actual_str, original);

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(eq(str, expected_str, original), "Expected autograder buffer to stay the same after execution");
    h_reverse_string(expected_str);

    cr_assert(eq(str, actual_str, expected_str), "Expected reversing string to be correct.");
}

Test(test_reverse_string, basic_5) {
    // Arguments:
    char *original = "123456";

    // Initialize strings:
    char expected_str[7];
    char actual_str[7];
    strcpy(expected_str, original);
    strcpy(actual_str, original);

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(eq(str, expected_str, original), "Expected autograder buffer to stay the same after execution");
    h_reverse_string(expected_str);

    cr_assert(eq(str, actual_str, expected_str), "Expected reversing string to be correct.");
}

Test(test_reverse_string, with_inner_null) {
    // Arguments:
    char *original = "12 \0 345";

    // Initialize strings:
    char expected_str[9];
    char actual_str[9];
    memcpy(expected_str, original, sizeof(expected_str));
    memcpy(actual_str, original, sizeof(actual_str));

    // Test `reverse_string`:
    reverse_string(actual_str);
    cr_assert(
        eq(mem,
           ((struct cr_mem){.data = expected_str, .size = sizeof(expected_str)}),
           ((struct cr_mem){.data = original, .size = sizeof(expected_str)})
        ),
        "Expected autograder buffer to stay the same after execution"
    );
    h_reverse_string(expected_str);

    cr_assert(eq(str, expected_str, actual_str), "Expected reversing string to be correct.");
}
