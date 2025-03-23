#include "../my_string.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_my_strlen, .timeout = UNREASONABLY_LONG);

Test(test_my_strlen, basic_1) {
    const char *string = "12345";

    size_t expected = strlen(string);
    size_t actual = my_strlen(string);
    cr_assert(eq(sz, actual, expected), "Expected length to be correct");
}

Test(test_my_strlen, basic_2) {
    const char *string = "WordsW0rdsWordsWords!";

    size_t expected = strlen(string);
    size_t actual = my_strlen(string);
    cr_assert(eq(sz, actual, expected), "Expected length to be correct");
}

Test(test_my_strlen, with_inner_null_1) {
    const char *string = "1\0";

    size_t expected = strlen(string);
    size_t actual = my_strlen(string);
    cr_assert(eq(sz, actual, expected), "Expected length to be correct");
}

Test(test_my_strlen, with_inner_null_2) {
    const char *string = "1\x0023";

    size_t expected = strlen(string);
    size_t actual = my_strlen(string);
    cr_assert(eq(sz, actual, expected), "Expected length to be correct");
}

Test(test_my_strlen, empty) {
    const char *string = "";

    size_t expected = strlen(string);
    size_t actual = my_strlen(string);
    cr_assert(eq(sz, actual, expected), "Expected length to be correct");
}
