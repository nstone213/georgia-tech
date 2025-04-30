#include "../my_malloc.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //

/*Initial Realloc*/
void *setup_realloc_init(void) {
    init_malloc_test(0, true);
    my_malloc_errno = OUT_OF_MEMORY;
    return my_realloc(NULL, 1024);
}

static metadata_t *meta;
/*Realloc Zero*/
void *setup_realloc_no_alloc(void) {
    init_malloc_test(1, false);

    // Put something in the freelist to check they don't touch it
    meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = SBRK_SIZE;
    meta->next = NULL;
    address_list = meta;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_realloc(NULL, 0);
}

/*Copy Realloc*/
char *important_data;
static unsigned long a_size;
static unsigned long b_size;
static unsigned long c_size;
void *setup_realloc_grow(void) {
    init_malloc_test(1, false);

    // Counting the null terminator, this is 55 bytes
    important_data = "Watch your fingers 'cause the cactus dangerous (yeah)\n";

    // Setup the freelist and fake heap to represent situation 1
    create_situation(&A, &B, &C);
    a_size = A->size;
    b_size = B->size;
    c_size = C->size;

    // Put this important data in memory somewhere
    meta = (metadata_t *)((uint8_t *)B + B->size + 128);
    meta->size = 55;
    // Copy in our important data
    strcpy((char *)(meta + 1), important_data);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_realloc(meta + 1, 128);
}

/*Realloc a smaller block*/
uint8_t *byte_after_B;
void *setup_realloc_shrink(void) {
    init_malloc_test(1, false);

    // Counting the null terminator, this is 242 bytes
    important_data = "Uh, yeah, mansion it sit on the hill, (woo) after my last arrest (woo, woo)\n"
                     "[Individuals] out here in the field, don't need a mask or vest (brrt, brrt)\n"
                     "Uh, back to back Lambos, repeat it, you like to run when it's heated (run, back to back)\n";

    // Setup the freelist and fake heap to represent situation 1
    create_situation(&A, &B, &C);
    // I'm lazy, so shrink A a little bit so that it'll be first in the new
    // freelist instead of the block passed in
    A->size -= 64;
    a_size = A->size;
    b_size = B->size;
    c_size = C->size;

    // Put this important data in memory somewhere
    meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = 242;
    // Copy in our important data
    strcpy((char *)(meta + 1), important_data);
    // B will be chosen, so to make sure they're not writing past the end of B,
    // zero out the byte directly following B and check that it stays zero.
    byte_after_B = (uint8_t *)B + b_size + TOTAL_METADATA_SIZE;
    *byte_after_B = 0;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_realloc(meta + 1, 64);
}

/*Double Merge 2 - Realloc*/
static metadata_t *prev_b;
void *setup_realloc_free(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation(&A, &B, &C);
    prev_b = B;
    b_size = B->size;
    c_size = C->size;
    meta = (metadata_t *)((uint8_t *)B + B->size + TOTAL_METADATA_SIZE);

    meta->size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;

    return my_realloc(meta + 1, 0);
}

/*TOOBIG*/
void *setup_realloc_clobber(void) {
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    return my_realloc(NULL, SBRK_SIZE);
}

/*OOM*/
void *setup_realloc_oom(void) {
    init_malloc_test(HEAP_SIZE / SBRK_SIZE, true);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = NO_ERROR;
    return my_realloc(NULL, 8);
}

// ********************************************************************************************************************* //
// *********************************************** realloc() TESTS ***************************************************** //
// ********************************************************************************************************************* //

TestSuite(test_my_realloc, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

/** Basic test where buffer grows **/

Test(test_my_realloc, alloc_grow_retvalue) {
    void *ret = setup_realloc_grow();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_realloc to return non-NULL pointer");
    uint8_t *split_right_choice = (uint8_t *)A + a_size - 128 + sizeof(metadata_t);
    cr_assert(eq(ptr, ret, split_right_choice), "Returned pointer is incorrect");
    cr_assert(zero(int, (strncmp((char *)ret, important_data, 55))), "Expected return to match data");
}

Test(test_my_realloc, alloc_grow_lists) {
    setup_realloc_grow();

    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");

    cr_assert(eq(ptr, address_list, A), "Returned pointer is incorrect");
    cr_assert(eq(ptr, A->next, B), "Returned pointer is incorrect");
    cr_assert(eq(ptr, B->next, meta), "Returned pointer is incorrect");
    cr_assert(eq(ptr, meta->next, C), "Returned pointer is incorrect");
    cr_assert(zero(ptr, C->next), "Expected returned pointer to be NULL");

    cr_assert(eq(int, A->size, a_size - 128 - TOTAL_METADATA_SIZE), "Incorrect block size at address_list");
    cr_assert(eq(int, B->size, b_size), "Incorrect block size at address_list");
    cr_assert(eq(int, C->size, c_size), "Incorrect block size at address_list");
}

/** Basic test where buffer shrinks **/

Test(test_my_realloc, alloc_shrink_retvalue) {
    void *ret = setup_realloc_shrink();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected address_list to point somewhere");
    metadata_t *ret_meta = (metadata_t *)ret - 1;
    cr_assert(eq(ptr, ret_meta, B), "Returned pointer is incorrect");
    cr_assert(eq(int, ret_meta->size, b_size), "Incorrect block size stored in metadata");

    cr_assert(zero(int, (strncmp((char *)ret, important_data, 64))), "Expected return to match data");
    cr_assert(not(eq(int, *byte_after_B, important_data[64 + sizeof(unsigned long)])), "Expected byte after B to not be overwritten");
}

Test(test_my_realloc, alloc_shrink_lists) {
    setup_realloc_shrink();

    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(eq(ptr, address_list, meta), "Returned pointer is incorrect");
    cr_assert(eq(ptr, meta->next, A), "Incorrect block size at address_list");
    cr_assert(eq(ptr, A->next, C), "Incorrect block size at address_list");
    cr_assert(zero(ptr, C->next), "Expected address_list->next->next to be NULL");

    cr_assert(eq(int, A->size, a_size), "Incorrect block size at address_list");
    cr_assert(eq(int, meta->size, 242), "Incorrect block size at address_list");
    cr_assert(eq(int, C->size, c_size), "Incorrect block size at address_list");
}

/** Basic test with NULL start **/

Test(test_my_realloc, alloc_init_retvalue) {
    void *ret = setup_realloc_init();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, ret)), "Expected my_realloc to return non-NULL pointer");
    uint8_t *split_right_choice = my_sbrk_fake_heap + SBRK_SIZE - 1024;
    cr_assert(eq(ptr, ret, split_right_choice), "Incorrect block address at return pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
}

Test(test_my_realloc, alloc_init_metadata) {
    void *ret = setup_realloc_init();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    metadata_t *meta = (metadata_t *)ret - 1;
    cr_assert(eq(int, meta->size, 1024), "Expected correct metadata size");
}

Test(test_my_realloc, alloc_init_sbrkcall) {
    setup_realloc_init();
    cr_assert(my_sbrk_called, "Expected my_sbrk() to be called");
}

Test(test_my_realloc, alloc_init_lists) {
    setup_realloc_init();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");

    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(zero(ptr, address_list->next), "Expected address_list->next to be NULL");
    cr_assert(eq(int, address_list->size, SBRK_SIZE - 1024 - (TOTAL_METADATA_SIZE * 2)), "Incorrect block size at address_list");
    cr_assert(eq(ptr, (uint8_t *)address_list, my_sbrk_fake_heap), "Returned pointer is incorrect");
}

/** realloc(ptr, 0) == freeing ptr **/

Test(test_my_realloc, alloc_free_retvalue) {
    void *ret = setup_realloc_free();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(zero(ptr, ret), "Expected my_realloc to return NULL pointer");
}

Test(test_my_realloc, alloc_free_lists) {
    setup_realloc_free();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(eq(ptr, A, address_list), "Incorrect block address at address_list");
    cr_assert(eq(ptr, A->next, B), "Incorrect block address at address_list->nxt");
    cr_assert(zero(ptr, B->next), "Expected address_list->next->next to be NULL");
}

Test(test_my_realloc, alloc_free_insert) {
    setup_realloc_free();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    size_t meta_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    cr_assert(eq(int, B->size, b_size + meta_size + c_size + 2 * TOTAL_METADATA_SIZE), "Incorrect block size in freelist after merge");
    cr_assert(eq(ptr, B, prev_b), "Freed block was not correctly reinserted into the freelist");
}

/** realloc(NULL, 0) **/

Test(test_my_realloc, no_alloc_retvalue) {
    void *ret = setup_realloc_no_alloc();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(zero(ptr, ret), "Expected my_realloc to return non-NULL pointer");
}

Test(test_my_realloc, no_alloc_lists) {
    setup_realloc_no_alloc();

    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, NO_ERROR), "Expected correct my_malloc_errno");
    cr_assert(not(zero(ptr, address_list)), "Expected address_list to point somewhere");
    cr_assert(zero(ptr, address_list->next), "Expected address_list->next to be NULL");
    cr_assert(eq(ptr, address_list, meta), "Returned pointer is incorrect");
    cr_assert(eq(int, address_list->size, SBRK_SIZE), "Incorrect block size at address_list");
}

/** realloc with too large of a request **/

Test(test_my_realloc, clobber) {
    void *ret = setup_realloc_clobber();

    cr_assert(zero(ptr, ret), "Expected my_realloc to return NULL pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, SINGLE_REQUEST_TOO_LARGE), "Expected correct my_malloc_errno");
}

/** realloc when OOM **/
Test(test_my_realloc, oom) {
    void *ret = setup_realloc_oom();

    cr_assert(zero(ptr, ret), "Expected my_realloc to return NULL pointer");
    cr_assert(eq(type(my_malloc_err_t), my_malloc_errno, OUT_OF_MEMORY), "Expected correct my_malloc_errno");
}
