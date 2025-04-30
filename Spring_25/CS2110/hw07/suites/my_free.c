#include "../my_malloc.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_my_free, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

// --------------------------- SETUP FUNCTIONS ---------------------------

static metadata_t *setup_empty_freelist(void) {
    metadata_t *meta = (metadata_t *)(my_sbrk_fake_heap + 64);
    meta->size = 128 + TOTAL_METADATA_SIZE;
    my_free(meta + 1);
    return meta;
}

static metadata_t *setup_no_merge_1(void) {
    create_situation(&A, &B, &C);
    metadata_t *meta = (metadata_t *)((uint8_t *)B + B->size + 8 + TOTAL_METADATA_SIZE);
    meta->size = 32;
    my_free(meta + 1);
    return meta;
}

static metadata_t *setup_no_merge_2(void) {
    create_situation(&A, &B, &C);
    metadata_t *meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = 400;
    my_free(meta + 1);
    return meta;
}

static metadata_t *setup_left_merge_1(size_t *a_size_out) {
    create_situation(&A, &B, &C);
    *a_size_out = A->size;
    metadata_t *meta = (metadata_t *)((uint8_t *)A + A->size + TOTAL_METADATA_SIZE);
    meta->size = 64;
    my_free(meta + 1);
    return A;
}

static metadata_t *setup_left_merge_2(size_t *b_size_out) {
    create_situation(&A, &B, &C);
    *b_size_out = B->size;
    B->next = NULL;
    metadata_t *meta = (metadata_t *)((uint8_t *)B + B->size + TOTAL_METADATA_SIZE);
    meta->size = SBRK_SIZE - 1472 - 2 * TOTAL_METADATA_SIZE;
    my_free(meta + 1);
    return B;
}

static metadata_t *setup_right_merge_1(size_t *b_size_out) {
    create_situation(&A, &B, &C);
    *b_size_out = B->size;
    metadata_t *meta = (metadata_t *)((uint8_t *)B - 64 - TOTAL_METADATA_SIZE);
    meta->size = 64;
    my_free(meta + 1);
    return meta;
}

static metadata_t *setup_right_merge_2(size_t *c_size_out) {
    create_situation(&A, &B, &C);
    *c_size_out = C->size;
    metadata_t *meta = (metadata_t *)((uint8_t *)C - 255);
    meta->size = 255 - TOTAL_METADATA_SIZE;
    my_free(meta + 1);
    return meta;
}

static metadata_t *setup_double_merge_1(size_t *expected_size_out) {
    create_situation(&A, &B, &C);
    size_t a_size = A->size;
    size_t b_size = B->size;
    size_t meta_size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;
    *expected_size_out = a_size + b_size + meta_size + 2 * TOTAL_METADATA_SIZE;
    metadata_t *meta = (metadata_t *)((uint8_t *)A + A->size + TOTAL_METADATA_SIZE);
    meta->size = meta_size;
    my_free(meta + 1);
    return A;
}

static metadata_t *setup_double_merge_2(size_t *expected_size_out) {
    create_situation(&A, &B, &C);
    size_t b_size = B->size;
    size_t c_size = C->size;
    size_t meta_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    *expected_size_out = b_size + c_size + meta_size + 2 * TOTAL_METADATA_SIZE;
    metadata_t *meta = (metadata_t *)((uint8_t *)B + B->size + TOTAL_METADATA_SIZE);
    meta->size = meta_size;
    my_free(meta + 1);
    return B;
}

static metadata_t *setup_double_decker(size_t *expected_size_out) {
    create_situation(&A, &B, &C);
    size_t a_size = A->size;
    size_t b_size = B->size;
    size_t c_size = C->size;

    metadata_t *meta2 = (metadata_t *)((uint8_t *)B + B->size + TOTAL_METADATA_SIZE);
    size_t meta_1_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    meta2->size = meta_1_size;
    my_free(meta2 + 1);

    metadata_t *meta1 = (metadata_t *)((uint8_t *)A + A->size + TOTAL_METADATA_SIZE);
    size_t meta_2_size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;
    meta1->size = meta_2_size;
    my_free(meta1 + 1);

    *expected_size_out = a_size + b_size + c_size + meta_1_size + meta_2_size + 4 * TOTAL_METADATA_SIZE;
    return A;
}

// --------------------------- TEST CASES ---------------------------

Test(test_my_free, null_ptr) {
    my_malloc_errno = OUT_OF_MEMORY;
    my_free(NULL);
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
}

Test(test_my_free, empty_freelist) {
    my_malloc_errno = OUT_OF_MEMORY;
    metadata_t *meta = setup_empty_freelist();

    // Check lists are correct
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    cr_assert(eq(ptr, meta, my_sbrk_fake_heap + 64));
    cr_assert(eq(int, meta->size, 128 + TOTAL_METADATA_SIZE));
    // Check remainder of freelist is correct
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to be non-null");
    cr_assert(eq(ptr, address_list, meta));
    cr_assert(zero(ptr, address_list->next));
}

Test(test_my_free, no_merge_1) {
    my_malloc_errno = OUT_OF_MEMORY;
    metadata_t *meta = setup_no_merge_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A));
    cr_assert(eq(ptr, A->next, B));
    cr_assert(eq(ptr, B->next, meta));
    cr_assert(eq(ptr, meta->next, C));
    cr_assert(zero(ptr, C->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, meta->size, 32));
    cr_assert(eq(ptr, meta, (uint8_t *)B + B->size + 8 + TOTAL_METADATA_SIZE));
}

Test(test_my_free, no_merge_2) {
    my_malloc_errno = OUT_OF_MEMORY;
    metadata_t *meta = setup_no_merge_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, meta));
    cr_assert(eq(ptr, meta->next, A));
    cr_assert(eq(ptr, A->next, B));
    cr_assert(eq(ptr, B->next, C));
    cr_assert(zero(ptr, C->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, meta->size, 400));
    cr_assert(eq(ptr, meta, my_sbrk_fake_heap));
}

Test(test_my_free, left_merge_1) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t a_size;
    metadata_t *A_ptr = setup_left_merge_1(&a_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A_ptr));
    cr_assert(eq(ptr, A_ptr->next, B));
    cr_assert(eq(ptr, B->next, C));
    cr_assert(zero(ptr, C->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, A_ptr->size, a_size + 64 + TOTAL_METADATA_SIZE));
}

Test(test_my_free, left_merge_2) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t b_size;
    metadata_t *B_ptr = setup_left_merge_2(&b_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A));
    cr_assert(eq(ptr, A->next, B_ptr));
    cr_assert(zero(ptr, B_ptr->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, B_ptr->size, b_size + (SBRK_SIZE - 1472 - 2 * TOTAL_METADATA_SIZE) + TOTAL_METADATA_SIZE));
}

Test(test_my_free, right_merge_1) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t b_size;
    metadata_t *meta = setup_right_merge_1(&b_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A));
    cr_assert(eq(ptr, A->next, meta));
    cr_assert(eq(ptr, meta->next, C));
    cr_assert(zero(ptr, C->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, meta->size, b_size + 64 + TOTAL_METADATA_SIZE));
}

Test(test_my_free, right_merge_2) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t c_size;
    metadata_t *meta = setup_right_merge_2(&c_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A));
    cr_assert(eq(ptr, A->next, B));
    cr_assert(eq(ptr, B->next, meta));
    cr_assert(zero(ptr, meta->next));
    // Check remainder of freelist is correct
    cr_assert(eq(ptr, meta, (uint8_t *)C - 255));
    cr_assert(eq(int, meta->size, c_size + 255));
}

Test(test_my_free, double_merge_1) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t expected_size;
    metadata_t *A_ptr = setup_double_merge_1(&expected_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A_ptr));
    cr_assert(eq(ptr, A_ptr->next, C));
    cr_assert(zero(ptr, C->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, A_ptr->size, expected_size));
}

Test(test_my_free, double_merge_2) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t expected_size;
    metadata_t *B_ptr = setup_double_merge_2(&expected_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A));
    cr_assert(eq(ptr, A->next, B_ptr));
    cr_assert(zero(ptr, B_ptr->next));
    // Check remainder of freelist is correct
    cr_assert(eq(int, B_ptr->size, expected_size));
}

Test(test_my_free, double_decker) {
    my_malloc_errno = OUT_OF_MEMORY;
    size_t expected_size;
    metadata_t *A_ptr = setup_double_decker(&expected_size);

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR));
    // Check lists are correct
    cr_assert(eq(ptr, address_list, A_ptr));
    cr_assert(eq(ptr, A_ptr, A));
    // Check remainder of freelist is correct
    cr_assert(eq(int, A_ptr->size, expected_size));
    cr_assert(zero(ptr, B->next));
    cr_assert(zero(ptr, C->next));
}
