#include "../my_malloc.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_my_calloc, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

// --------------------------- SETUP FUNCTIONS ---------------------------

static uint8_t *setup_calloc_basic(void) {
    init_malloc_test(0, true);
    memset(my_sbrk_fake_heap, 0x69, SBRK_SIZE);
    my_malloc_errno = OUT_OF_MEMORY;
    return my_calloc(8, 64);
}

static void *setup_calloc_no_alloc(void) {
    init_malloc_test(0, false);
    my_malloc_errno = OUT_OF_MEMORY;
    return my_calloc(0, 69);
}

static void *setup_calloc_clobber(void) {
    init_malloc_test(0, false);
    my_malloc_errno = OUT_OF_MEMORY;
    return my_calloc(SBRK_SIZE, 1);
}

static uint8_t *setup_calloc_mem_zeroed(void) {
    init_malloc_test(1, false);
    memset(my_sbrk_fake_heap, 0x69, SBRK_SIZE);
    create_situation(&A, &B, &C);
    my_malloc_errno = OUT_OF_MEMORY;
    return my_calloc(1, 607);
}

// --------------------------- TEST CASES ---------------------------

Test(test_my_calloc, basic_retvalue) {
    uint8_t *ret = setup_calloc_basic();

    // Check right pointer was returned
    cr_assert(not(zero(ret)), "Expected my_calloc to return non-NULL pointer");
    uint8_t *split_right_choice = my_sbrk_fake_heap + SBRK_SIZE - 8 * 64;
    cr_assert(eq(ptr, ret, split_right_choice));

    // Assert data is zeroed out
    char expected_data[512] = {0};
    struct cr_mem expected = {.data = expected_data, .size = 512};
    struct cr_mem actual = {.data = ret, .size = 512};
    cr_assert(eq(mem, actual, expected), "Expected memory to be zeroed out");

    // Check error is correct
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct `my_malloc_errno`");
}

// Assert correct metadata
Test(test_my_calloc, basic_metadata) {
    uint8_t *ret = setup_calloc_basic();

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 512), "Expected metadata size to be correct");
}

// Assert my_sbrk() was called
Test(test_my_calloc, basic_sbrkcall) {
    setup_calloc_basic();

    cr_assert(my_sbrk_called, "Expected my_sbrk() to be called");
}

// Assert correct freelists
Test(test_my_calloc, basic_lists) {
    setup_calloc_basic();

    cr_assert(not(zero(address_list)), "Expected address_list to point somewhere");
    cr_assert(zero(address_list->next), "Expected address_list->next to be NULL");
    cr_assert(eq(int, address_list->size, SBRK_SIZE - 512 - (TOTAL_METADATA_SIZE * 2)), "Incorrect block size at address_list");
    cr_assert(eq(ptr, address_list, my_sbrk_fake_heap), "Incorrect block address at address_list");
}

Test(test_my_calloc, no_alloc) {
    void *ret = setup_calloc_no_alloc();

    cr_assert(eq(ptr, ret, NULL), "Expected NULL return when allocating zero bytes");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected my_malloc_errno to be NO_ERROR");
}

Test(test_my_calloc, clobber) {
    void *ret = setup_calloc_clobber();

    cr_assert(eq(ptr, ret, NULL), "Expected NULL when allocation exceeds SBRK_SIZE");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, SINGLE_REQUEST_TOO_LARGE), "Expected SINGLE_REQUEST_TOO_LARGE errno");
}

Test(test_my_calloc, mem_zeroed_retvalue) {
    uint8_t *ret = setup_calloc_mem_zeroed();

    // Check right pointer was returned
    cr_assert(not(zero(ret)), "Expected non-NULL from my_calloc");
    uint8_t *expected_ptr = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE + (C_START_SIZE - 607);
    cr_assert(eq(ptr, ret, expected_ptr));

    // Assert data is zeroed out
    char expected_data[607] = {0};
    struct cr_mem expected = {.data = expected_data, .size = 607};
    struct cr_mem actual = {.data = ret, .size = 607};
    cr_assert(eq(mem, actual, expected), "Expected memory to be zeroed out");

    // Check error is correct
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected my_malloc_errno to be NO_ERROR");
}

Test(test_my_calloc, mem_zeroed_splitblock) {
    setup_calloc_mem_zeroed();

    // Assert correct address for C
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + C_START_OFF;
    cr_assert(eq(ptr, C, split_left), "Expected C to point to the correct location");

    // Check remainder is correct
    size_t expected_c_size = C_START_SIZE - 607 - TOTAL_METADATA_SIZE;
    cr_assert(eq(int, C->size, expected_c_size), "Expected correct size in remainder block");
}

Test(test_my_calloc, mem_zeroed_metadata) {
    uint8_t *ret = setup_calloc_mem_zeroed();

    // Assert metadata correct
    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 607), "Expected metadata size to be correct");
}

Test(test_my_calloc, mem_zeroed_lists) {
    setup_calloc_mem_zeroed();

    // Check free list is correct
    cr_assert(eq(ptr, address_list, A), "Expected A to be front of list");
    cr_assert(eq(ptr, A->next, B), "Expected B to follow A");
    cr_assert(eq(ptr, B->next, C), "Expected C to follow B");
    cr_assert_null(C->next, "Expected C to be last node");
}
