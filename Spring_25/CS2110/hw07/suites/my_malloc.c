#include "../my_malloc.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// Helpers:
bool my_sbrk_called;
bool my_sbrk_call_expected;
uint8_t *my_sbrk_fake_heap;       // Pointer to the beginning of the fake heap -- initiallized in my_sbrk
int my_sbrk_imaginary_prev_calls; // Determines the position of the break at the beginning of the tests

void *my_sbrk(int increment) {
    cr_assert(eq(int, increment, SBRK_SIZE));
    cr_assert(my_sbrk_call_expected, "There was a block available for this call to my_malloc, but you still made a call to my_sbrk when you did not have to");
    cr_assert(not(my_sbrk_called), "You should be only calling my_sbrk once in an invocation of my_malloc. Check your my_malloc code, it looks like you are calling my_sbrk more than once.");
    my_sbrk_called = true;

    // Handle OOM
    if (my_sbrk_imaginary_prev_calls >= HEAP_SIZE / SBRK_SIZE) {
        return (void *)-1;
    } else {
        return my_sbrk_fake_heap + my_sbrk_imaginary_prev_calls * SBRK_SIZE;
    }
}
void init_malloc_test(int prev_sbrk_calls, bool should_sbrk) {
    my_sbrk_imaginary_prev_calls = prev_sbrk_calls;
    my_sbrk_call_expected = should_sbrk;
}

void setup_heap(void) {
    address_list = NULL;
    my_sbrk_called = false;
    my_sbrk_fake_heap = calloc(1, HEAP_SIZE); // Allocates two pages each of 4096 bytes for the heap
    memset(my_sbrk_fake_heap, 0x69, HEAP_SIZE);
    cr_assert(not(zero(ptr, my_sbrk_fake_heap)), "Out of memory"); // Out of memory. Shouldn't happen but check anyway
}
void teardown_heap(void) {
    free(my_sbrk_fake_heap);
    my_sbrk_fake_heap = NULL;
}

// Create situation
void create_situation(metadata_t **Aout, metadata_t **Bout, metadata_t **Cout) {
    // Setup A
    metadata_t *A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->size = A_START_SIZE;

    // Setup B
    metadata_t *B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->size = B_START_SIZE;

    // Setup C
    metadata_t *C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->size = C_START_SIZE;

    // Setup linked_lists by address
    address_list = A;
    A->next = B;
    B->next = C;
    C->next = NULL;

    // Return all
    if (Aout) {
        *Aout = A;
    }
    if (Bout) {
        *Bout = B;
    }
    if (Cout) {
        *Cout = C;
    }
}

// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //

/* MALLOC_INIT */
void *setup_malloc_init(void) {
    init_malloc_test(0, true);
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(128);
}

/*PERFECT BLOCK 1*/
metadata_t *A, *B, *C;
void *setup_malloc_perf_block_1(void) {
    init_malloc_test(1, false);
    // change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation(&A, &B, &C);
    return my_malloc(C->size);
}

/*PERFECT BLOCK 2*/
void *setup_malloc_perf_block_2(void) {
    init_malloc_test(1, false);
    // change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation(&A, &B, &C);
    return my_malloc(B->size);
}

/*PERFECT BLOCK 3*/
void *setup_malloc_perf_block_3(void) {
    init_malloc_test(1, false);
    // change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation(&A, &B, &C);
    return my_malloc(A->size);
}

/*SPLIT BLOCK 1*/
void *setup_malloc_split_block_1(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(128);
}

/*SPLIT BLOCK 2*/
void *setup_malloc_split_block_2(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(260);
}

/*SPLIT BLOCK 3*/
void *setup_malloc_split_block_3(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(607);
}

void *setup_malloc_split_min_block(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick B and split when exactly MBS left
    return my_malloc(B_START_SIZE - MIN_BLOCK_SIZE);
}

void *setup_malloc_no_split_block_1(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick A but not split
    return my_malloc(A_START_SIZE - (MIN_BLOCK_SIZE - 1));
}

void *setup_malloc_no_split_block_2(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick B but not split
    // note that we add 1 here to provoke size_t underflow if you check if you
    // check if you should split like this:
    //   block->size - size - TMS >= MIN_BLOCK_SIZE - TMS;
    // instead, you should write this:
    //   block->size - size >= MIN_BLOCK_SIZE;
    return my_malloc(B_START_SIZE - (MIN_BLOCK_SIZE - 1) + 1);
}

void *setup_malloc_no_split_block_3(void) {
    init_malloc_test(1, false);

    create_situation(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick C but not split
    return my_malloc(C_START_SIZE - (MIN_BLOCK_SIZE - 1));
}

/* Split Block with SBRK MERGE */
metadata_t *D, *E;
void *setup_malloc_split_block_sbrk_merge(void) {
    init_malloc_test(1, true);
    D = (metadata_t *)(my_sbrk_fake_heap + D_START_OFF);
    D->size = D_START_SIZE;

    E = (metadata_t *)(my_sbrk_fake_heap + E_START_OFF);
    E->size = E_START_SIZE;

    address_list = D;
    D->next = E;
    E->next = NULL;

    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(256);
}

/* SBRK WORKS */
void *setup_malloc_init_sbrk(void) {
    init_malloc_test(1, false);
    address_list = (metadata_t *)my_sbrk_fake_heap;
    address_list->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
    address_list->next = NULL;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(64);
}

/*OOM*/
void *setup_malloc_oom(void) {
    init_malloc_test(HEAP_SIZE / SBRK_SIZE, true);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = NO_ERROR;
    return my_malloc(8);
}

/*ZERO*/
void *setup_malloc_no_alloc(void) {
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(0);
}

/*TOOBIG*/
void *setup_malloc_clobber(void) {
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_malloc(SBRK_SIZE);
}

// ********************************************************************************************************************* //
// *********************************************** malloc() TESTS ****************************************************** //
// ********************************************************************************************************************* //

TestSuite(test_my_malloc, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

/***** Malloc_Init Test Case Tests *****/

Test(test_my_malloc, alloc_init_retvalue) {
    void *ret = setup_malloc_init();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right_choice = my_sbrk_fake_heap + SBRK_SIZE - 128;
    cr_assert(eq(ptr, ret, split_right_choice), "Returned pointer is incorrect");
}

Test(test_my_malloc, alloc_init_metadata) {
    void *ret = setup_malloc_init();

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 128), "Expected correct metadata size");
}

Test(test_my_malloc, alloc_init_sbrkcall) {
    setup_malloc_init();
    cr_assert(my_sbrk_called, "Expected my_sbrk() to be called");
}

Test(test_my_malloc, alloc_init_lists) {
    setup_malloc_init();

    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(zero(ptr, address_list->next), "Expected address_list->next to be NULL");
    cr_assert(eq(int, address_list->size, SBRK_SIZE - 128 - (TOTAL_METADATA_SIZE * 2)), "Incorrect block size at address_list");

    cr_assert(eq(ptr, address_list, my_sbrk_fake_heap), "Incorrect block address at address_list");
}

/***** Malloc_Init_SBRK Test Case Tests *****/

Test(test_my_malloc, alloc_init_sbrk_retvalue) {
    void *ret = setup_malloc_init_sbrk();

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right_choice = my_sbrk_fake_heap + SBRK_SIZE - 64;
    cr_assert(eq(ret, split_right_choice), "Returned pointer is incorrect");
}

// checks to see if the metadata of the block returned to the user is correct
Test(test_my_malloc, alloc_init_sbrk_metadata) {
    void *ret = setup_malloc_init_sbrk();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 64), "Expected correct metadata size");
}

// checks to see if address_list are correct
Test(test_my_malloc, alloc_init_sbrk_lists) {
    setup_malloc_init_sbrk();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(zero(ptr, address_list->next), "Expected address_list->next to be NULL");
    cr_assert(eq(int, address_list->size, SBRK_SIZE - 64 - (TOTAL_METADATA_SIZE * 2)), "Incorrect block size at address_list");
    cr_assert(eq(ptr, address_list, my_sbrk_fake_heap), "Incorrect block address at address_list");
}

/***** Malloc_Perf_Block1 Test Case Tests *****/

// checks to see if address_list are correct
Test(test_my_malloc, perf_block_1_retvalue) {
    void *ret = setup_malloc_perf_block_1();

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    cr_assert(eq(ptr, ret, (C + 1)), "Incorrect block address at return pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
}

// checking to see if the metadata of the block returned to the user is correct
Test(test_my_malloc, perf_block_1_metadata) {
    void *ret = setup_malloc_perf_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, C->size), "Expected correct metadata size");
}

// checks to see if address_list are correct
Test(test_my_malloc, perf_block_1_lists) {
    setup_malloc_perf_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    // check address_list front to back
    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, B->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_Perf_Block2 Test Case Tests *****/

// checks to see if returned right block and proper error code set
Test(test_my_malloc, perf_block_2_retvalue) {
    void *ret = setup_malloc_perf_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    cr_assert(eq(ptr, ret, (B + 1)), "Incorrect block address at return pointer");
}
// checks to see if metadata of the returned block is correct
Test(test_my_malloc, perf_block_2_metadata) {
    void *ret = setup_malloc_perf_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, B->size), "Expected correct metadata size");
}

// checks to see if address_list is correct
Test(test_my_malloc, perf_block_2_lists) {
    setup_malloc_perf_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    // check address_list front to back
    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, C), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_Perf_Block3 Test Case Tests *****/

// checks to see if returned right block
Test(test_my_malloc, perf_block_3_retvalue) {
    void *ret = setup_malloc_perf_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    cr_assert(eq(ptr, ret, (A + 1)), "Incorrect block address at return pointer");
}

// checks to make sure metadata of returned block is correct
Test(test_my_malloc, perf_block_3_metadata) {
    void *ret = setup_malloc_perf_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, A->size), "Expected correct metadata size");
}

Test(test_my_malloc, perf_block_3_lists) {
    setup_malloc_perf_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    // check address_list front to back
    cr_assert(eq(ptr, B, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_Split_Block1 Test Case Tests *****/

// checking to see if you returned the correct block
Test(test_my_malloc, split_block_1_retvalue) {
    void *ret = setup_malloc_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right = my_sbrk_fake_heap + A_START_OFF + TOTAL_METADATA_SIZE + A_START_SIZE - 128;
    cr_assert(eq(ptr, ret, split_right), "Incorrect block address at return pointer");
}

// making sure the remainder of the split block is correct in the freelist`
Test(test_my_malloc, split_block_1_splitblock) {
    setup_malloc_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + A_START_OFF;
    cr_assert(eq(ptr, A, split_left), "Incorrect block address at return pointer");
    cr_assert(eq(int, A->size, A_START_SIZE - 128 - TOTAL_METADATA_SIZE), "Incorrect remainder of split block in list");
}

// checking to see if the metadata of the returned block is correct
Test(test_my_malloc, split_block_1_metadata) {
    void *ret = setup_malloc_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 128), "Expected correct metadata size");
}

// checking to see if the lists are correct after the operation
Test(test_my_malloc, split_block_1_lists) {
    setup_malloc_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    // check address_list front to back
    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next->next to be NULL");
}

/***** Malloc_Split_Block2 Test Case Tests *****/

// checking to see if the return value is correct
Test(test_my_malloc, split_block_2_retvalue) {
    void *ret = setup_malloc_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE + (C_START_SIZE - 260);
    cr_assert(eq(ptr, ret, split_right), "Incorrect block address at return pointer");
}

// checking to see if the remainder is correct in the freelist
Test(test_my_malloc, split_block_2_splitblock) {
    setup_malloc_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + C_START_OFF;
    cr_assert(eq(ptr, C, split_left), "Incorrect block address at return pointer");
    unsigned long c_size = C_START_SIZE - 260 - TOTAL_METADATA_SIZE;
    cr_assert(eq(int, C->size, c_size), "Incorrect remainder of split block in list");
}

// checking to see if the metadata of the returned block is correct
Test(test_my_malloc, split_block_2_metadata) {
    void *ret = setup_malloc_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 260), "Expected correct metadata size");
}

// check to see if freelists are correct
Test(test_my_malloc, split_block_2_lists) {
    setup_malloc_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to be non-NULL pointer");

    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next->next to be NULL");
}

/***** Malloc_Split_Block3 Test Case Tests *****/

// check retvalue correct
Test(test_my_malloc, split_block_3_retvalue) {
    void *ret = setup_malloc_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE + (C_START_SIZE - 607);
    cr_assert(eq(ptr, ret, split_right), "Incorrect block address at return pointer");
}

// check remainder of block in freelist is correct
Test(test_my_malloc, split_block_3_splitblock) {
    setup_malloc_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + C_START_OFF;
    cr_assert(eq(ptr, C, split_left), "Incorrect block address at return pointer");
    unsigned long c_size = C_START_SIZE - 607 - TOTAL_METADATA_SIZE;
    cr_assert(eq(int, C->size, c_size), "Incorrect remainder of split block in list");
}

// check metadata of block returned correct
Test(test_my_malloc, split_block_3_metadata) {
    void *ret = setup_malloc_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 607), "Expected correct metadata size");
}

// check for correct freelist
Test(test_my_malloc, split_block_3_lists) {
    setup_malloc_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to be non-NULL pointer");

    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next->next to be NULL");
}

/***** Malloc_Split_Min_Block_Size Test Case Tests *****/

Test(test_my_malloc, split_min_block_retvalue) {
    void *ret = setup_malloc_split_min_block();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *split_right = my_sbrk_fake_heap + B_START_OFF + MIN_BLOCK_SIZE + TOTAL_METADATA_SIZE;
    cr_assert(eq(ptr, ret, split_right), "Incorrect block address at return pointer");
}

Test(test_my_malloc, split_min_block_splitblock) {
    setup_malloc_split_min_block();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + A_START_OFF;

    cr_assert(eq(ptr, A, split_left), "Incorrect block address at return pointer");
    cr_assert(eq(int, B->size, MIN_BLOCK_SIZE - TOTAL_METADATA_SIZE), "Incorrect remainder of split block in list");
}

Test(test_my_malloc, split_min_block_metadata) {
    void *ret = setup_malloc_split_min_block();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, B_START_SIZE - MIN_BLOCK_SIZE), "Expected correct metadata size");
}

Test(test_my_malloc, split_min_block_lists) {
    setup_malloc_split_min_block();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to be non-NULL pointer");

    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next->next to be NULL");
}

/***** Malloc_No_Split_Block1 Test Case Tests *****/

Test(test_my_malloc, no_split_block_1_retvalue) {
    void *ret = setup_malloc_no_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *expected = my_sbrk_fake_heap + A_START_OFF + TOTAL_METADATA_SIZE;
    cr_assert(eq(ptr, ret, expected), "Expected different block address at return pointer");
}

Test(test_my_malloc, no_split_block_1_metadata) {
    void *ret = setup_malloc_no_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, A_START_SIZE), "Expected correct metadata size");
}

Test(test_my_malloc, no_split_block_1_lists) {
    setup_malloc_no_split_block_1();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(eq(ptr, B, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, B->next, C), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_No_Split_Block2 Test Case Tests *****/

Test(test_my_malloc, no_split_block_2_retvalue) {
    void *ret = setup_malloc_no_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *expected = my_sbrk_fake_heap + B_START_OFF + TOTAL_METADATA_SIZE;
    cr_assert(eq(ptr, ret, expected), "Expected different block address at return pointer");
}

Test(test_my_malloc, no_split_block_2_metadata) {
    void *ret = setup_malloc_no_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, B_START_SIZE), "Expected correct metadata size");
}

Test(test_my_malloc, no_split_block_2_lists) {
    setup_malloc_no_split_block_2();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, C), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_No_Split_Block3 Test Case Tests *****/

Test(test_my_malloc, no_split_block_3_retvalue) {
    void *ret = setup_malloc_no_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *expected = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE;
    cr_assert(eq(ptr, ret, expected), "Expected different block address at return pointer");
}

Test(test_my_malloc, no_split_block_3_metadata) {
    void *ret = setup_malloc_no_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, C_START_SIZE), "Expected correct metadata size");
}

Test(test_my_malloc, no_split_block_3_lists) {
    setup_malloc_no_split_block_3();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, B->next), "Expected address_list->next->next to be NULL");
}

/***** Malloc_Split_Block_SBRKmerge Test Case Tests *****/

Test(test_my_malloc, split_block_sbrk_merge_sbrkcalled) {
    setup_malloc_split_block_sbrk_merge();

    cr_assert(my_sbrk_called, "Expected my_sbrk() to be called");
}

Test(test_my_malloc, split_block_sbrk_merge_retvalue) {
    void *ret = setup_malloc_split_block_sbrk_merge();

    cr_assert(not(zero(ptr, ret)), "Expected my_malloc to return non-NULL pointer");
    uint8_t *pointer = my_sbrk_fake_heap + 2 * SBRK_SIZE - 256 - TOTAL_METADATA_SIZE + sizeof(metadata_t);
    cr_assert(eq(ptr, ret, pointer), "Incorrect block address at return pointer");
}

Test(test_my_malloc, split_block_sbrk_merge_metadata) {
    void *ret = setup_malloc_split_block_sbrk_merge();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 256), "Expected correct metadata size");
}

Test(test_my_malloc, split_block_sbrk_merge_splitblock) {
    setup_malloc_split_block_sbrk_merge();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    uint8_t *split_left = (uint8_t *)my_sbrk_fake_heap + SBRK_SIZE - 64 - TOTAL_METADATA_SIZE;
    cr_assert(eq(ptr, E, split_left), "Incorrect block address at return pointer");

    unsigned long e_size = SBRK_SIZE - 192 - TOTAL_METADATA_SIZE;
    cr_assert(eq(int, E->size, e_size), "Incorrect remainder of split block in list");
}

Test(test_my_malloc, split_block_sbrk_merge_lists) {
    setup_malloc_split_block_sbrk_merge();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(eq(ptr, address_list, D), "Incorrect block address at address_list");
    cr_assert(eq(ptr, D->next, E), "Incorrect block address at address_list->next");
    cr_assert(zero(ptr, E->next), "Expected address_list->next->next->next to be NULL");
}

/***** Malloc_Zero Test Case Test *****/
Test(test_my_malloc, no_alloc) {
    void *ret = setup_malloc_no_alloc();

    cr_assert(zero(ptr, ret), "Expected my_malloc to return NULL pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
}

/***** Malloc_TooBig Test Case Test *****/
Test(test_my_malloc, clobber) {
    void *ret = setup_malloc_clobber();

    cr_assert(zero(ptr, ret), "Expected my_malloc to return NULL pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, SINGLE_REQUEST_TOO_LARGE), "Expected correct my_malloc_errno");
}

/***** Malloc_OOM Test Case Test *****/
Test(test_my_malloc, oom) {
    void *ret = setup_malloc_oom();

    cr_assert(zero(ptr, ret), "Expected my_malloc to return NULL pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, OUT_OF_MEMORY), "Expected correct my_malloc_errno");
}