#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_is_numeric, .timeout = UNREASONABLY_LONG);

Test(test_is_numeric, basic_1) {
    const char *string = "12345";

    int expected = is_numeric(string);
    int student = h_is_numeric(string);

    cr_assert(eq(int, expected, student), "Expected is_numeric to be correct.");
}

Test(test_is_numeric, basic_2) {
    const char *string = "12as";

    int expected = is_numeric(string);
    int student = h_is_numeric(string);

    cr_assert(eq(int, expected, student), "Expected is_numeric to be correct.");
}

Test(test_is_numeric, basic_3) {
    const char *string = "abcde";

    int expected = is_numeric(string);
    int student = h_is_numeric(string);

    cr_assert(eq(int, expected, student), "Expected is_numeric to be correct.");
}

Test(test_is_numeric, with_inner_null) {
    const char *string = "123\0abc";

    int expected = is_numeric(string);
    int student = h_is_numeric(string);

    cr_assert(eq(int, expected, student), "Expected is_numeric to be correct.");
}

Test(test_is_numeric, empty) {
    const char *string = "";

    int expected = is_numeric(string);
    int student = h_is_numeric(string);

    cr_assert(eq(int, expected, student), "Expected is_numeric to be correct.");
}