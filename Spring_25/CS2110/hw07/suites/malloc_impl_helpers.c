#include "../my_malloc.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_add_to_addr_list, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

Test(test_add_to_addr_list, empty) {
    metadata_t *new_block = (metadata_t *)my_sbrk_fake_heap;
    new_block->size = SBRK_SIZE;
    new_block->next = (metadata_t *)0x69;

    add_to_addr_list(new_block);
    cr_assert(eq(ptr, address_list, new_block), "Expected new block to be the head of the list");
    cr_assert(zero(ptr, new_block->next), "Expected new block's next to be NULL");
}
Test(test_add_to_addr_list, front) {
    create_situation(&A, &B, &C);

    // Create new block before A, B, C
    metadata_t *new_block = (metadata_t *)(my_sbrk_fake_heap + D_START_OFF);
    new_block->size = D_START_SIZE;
    new_block->next = (metadata_t *)0x69;

    add_to_addr_list(new_block);

    // New block is inserted at the front
    cr_assert(eq(ptr, address_list, new_block), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, new_block->next, A), "Expected blocks to be in correct order");
    // Rest of list is intact
    cr_assert(eq(ptr, A->next, B), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, B->next, C), "Expected blocks to be in correct order");
    cr_assert(zero(ptr, C->next), "Expected blocks to be in correct order");
}
Test(test_add_to_addr_list, middle) {
    create_situation(&A, &B, &C);

    // New block between B, C
    metadata_t *new_block = (metadata_t *)(my_sbrk_fake_heap + 1100);
    new_block->size = 64;
    new_block->next = (metadata_t *)0x69;

    add_to_addr_list(new_block);

    // New block is inserted between B, C
    cr_assert(eq(ptr, B->next, new_block), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, new_block->next, C), "Expected blocks to be in correct order");
    // Rest of list is intact
    cr_assert(eq(ptr, address_list, A), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, A->next, B), "Expected blocks to be in correct order");
    cr_assert(zero(ptr, C->next), "Expected blocks to be in correct order");
}
Test(test_add_to_addr_list, end) {
    create_situation(&A, &B, &C);

    // Create new block after A, B, C
    metadata_t *new_block = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF + C_START_SIZE + TOTAL_METADATA_SIZE + 100);
    new_block->size = 128;
    new_block->next = (metadata_t *)0x69;

    add_to_addr_list(new_block);

    // New block is inserted after A, B, C
    cr_assert(eq(ptr, C->next, new_block), "Expected blocks to be in correct order");
    cr_assert(zero(ptr, new_block->next), "Expected blocks to be in correct order");
    // Rest of list is intact
    cr_assert(eq(ptr, address_list, A), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, A->next, B), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, B->next, C), "Expected blocks to be in correct order");
}
Test(test_add_to_addr_list, single) {
    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->size = B_START_SIZE;
    B->next = NULL;
    address_list = B;

    // Assert C inserted after B
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->size = C_START_SIZE;
    C->next = (metadata_t *)0x69;
    add_to_addr_list(C);

    cr_assert(eq(ptr, address_list, B), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, B->next, C), "Expected blocks to be in correct order");
    cr_assert(zero(ptr, C->next), "Expected blocks to be in correct order");

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->size = A_START_SIZE;
    A->next = (metadata_t *)0x69;

    // Remove C, insert A
    B->next = NULL;
    add_to_addr_list(A);

    // Assert A inserted after B
    cr_assert(eq(ptr, address_list, A), "Expected blocks to be in correct order");
    cr_assert(eq(ptr, A->next, B), "Expected blocks to be in correct order");
    cr_assert(zero(ptr, B->next), "Expected blocks to be in correct order");
}

TestSuite(test_split_block, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

Test(test_split_block, basic_1) {
    create_situation(&A, &B, &C);
    metadata_t *new_block = split_block(A, 30);

    // Check A and new_block are contiguous
    cr_assert(eq(ptr, (uint8_t *)(A + 1) + A->size, (uint8_t *)new_block), "Expected split block to be contiguous with original");
    // Check new_block's size is correct
    cr_assert(eq(int, new_block->size, 30), "Expected split block's size to be correct");
    // Shortened A block is still in freelist
    cr_assert(eq(ptr, address_list, A), "Expected freelist blocks to be in correct order");
    cr_assert(eq(ptr, A->next, B), "Expected freelist blocks to be in correct order");
}
Test(test_split_block, basic_2) {
    create_situation(&A, &B, &C);
    metadata_t *new_block = split_block(B, 6);

    // Check B and new_block are contiguous
    cr_assert(eq(ptr, (uint8_t *)(B + 1) + B->size, (uint8_t *)new_block), "Expected split block to be contiguous with original");
    // Check new_block's size is correct
    // Shortened B block is still in freelist
    cr_assert(eq(int, new_block->size, 6), "Expected split block's size to be correct");
    cr_assert(eq(ptr, address_list, A), "Expected freelist blocks to be in correct order");
    cr_assert(eq(ptr, A->next, B), "Expected freelist blocks to be in correct order");
    cr_assert(eq(ptr, B->next, C), "Expected freelist blocks to be in correct order");
}

TestSuite(test_merge, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

Test(test_merge, basic_1) {
    metadata_t *left_block = (metadata_t *)(my_sbrk_fake_heap);
    left_block->size = 120;
    metadata_t *right_block = (metadata_t *)(my_sbrk_fake_heap + TOTAL_METADATA_SIZE + 120);
    right_block->size = 400;
    right_block->next = NULL;
    left_block->next = right_block;

    address_list = left_block;

    merge(left_block, right_block);

    // Assert left block still in list (and right block is removed)
    cr_assert(zero(ptr, left_block->next), "Did not expect merged block to have next");
    cr_assert(eq(ptr, address_list, left_block), "Expected block to still be in list");

    // Assert left block's size is set properly
    cr_assert(eq(int, left_block->size, 520 + TOTAL_METADATA_SIZE), "Expected merged block to have correct size");
}
Test(test_merge, basic_2) {
    metadata_t *block_one = (metadata_t *)(my_sbrk_fake_heap);
    block_one->size = 45;
    metadata_t *block_two = (metadata_t *)(my_sbrk_fake_heap + TOTAL_METADATA_SIZE + 45);
    block_two->size = 400;
    metadata_t *block_three = (metadata_t *)(my_sbrk_fake_heap + 2 * TOTAL_METADATA_SIZE + 45 + 400);
    block_three->size = 100;
    block_one->next = block_two;
    block_two->next = block_three;
    block_three->next = NULL;

    address_list = block_one;

    merge(block_one, block_two);

    // Assert block 1 still in list (with block 2 removed)
    cr_assert(eq(ptr, block_one->next, block_three), "Did not expect merged block to have next");
    cr_assert(eq(ptr, address_list, block_one), "Expected block to still be in list");

    // Assert block has correct size
    cr_assert(eq(int, block_one->size, 445 + TOTAL_METADATA_SIZE), "Expected merged block to have correct size");
}

TestSuite(test_find_best_fit, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

Test(test_find_best_fit, perfect_fit) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE;

    address_list = A;

    metadata_t *foundBlock = find_best_fit((size_t)(B_START_SIZE));

    // Tests
    cr_assert(eq(ptr, foundBlock, B), "Expected correct best fit block");
    cr_assert(eq(int, foundBlock->size, B->size), "Expected correct best fit block size");
    cr_assert(eq(ptr, foundBlock->next, B->next), "Expected correct best fit block next");
}
Test(test_find_best_fit, imperfect_fit) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE;

    address_list = A;

    metadata_t *foundBlock = find_best_fit((size_t)94);

    // Tests
    cr_assert(eq(ptr, foundBlock, A), "Incorrect best fit block");
    cr_assert(eq(int, foundBlock->size, A->size), "Incorrect best fit block size");
    cr_assert(eq(ptr, foundBlock->next, A->next), "Incorrect best fit block next");
}
Test(test_find_best_fit, no_fit) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE;

    address_list = A;

    metadata_t *foundBlock = find_best_fit((size_t)10000);

    // Tests
    cr_assert(zero(ptr, foundBlock), "Expected no fit to be found");
}
Test(test_find_best_fit, tiny_initial_block) {
    // case used to catch if the student improperly initializes the best fit
    // candidate to the first block in the free list (their implementation can
    // fail if the initial block is smaller than the requested size)

    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = 10;

    address_list = A;

    metadata_t *foundBlock = find_best_fit((size_t)(B_START_SIZE));

    // Tests
    cr_assert(eq(ptr, foundBlock, B), "Expected correct best fit block");
    cr_assert(eq(int, foundBlock->size, B->size), "Expected correct best fit block size");
    cr_assert(eq(ptr, foundBlock->next, B->next), "Expected correct best fit block next");
}

TestSuite(test_find_right, .timeout = UNREASONABLY_LONG, .init = setup_heap, .fini = teardown_heap);

Test(test_find_right, end_of_list) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE - TOTAL_METADATA_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE;

    address_list = B;

    metadata_t *found = find_right(C);
    // Tests
    cr_assert(zero(ptr, found), "Did not expect block after end of list");
}
Test(test_find_right, immediate) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->size = (SBRK_SIZE - A_START_SIZE - C_START_SIZE);
    C->next = NULL;

    B = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF + A_START_SIZE);
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;
    B->next = C;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE;

    address_list = A;

    metadata_t *found = find_right(A);
    // Tests
    cr_assert(eq(ptr, found, B), "Expected correct block right of A");
    cr_assert(eq(int, found->size, B->size), "Expected correct block right of A");
    cr_assert(eq(ptr, found->next, B->next), "Expected correct block right of A");
}
Test(test_find_right, none) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE - TOTAL_METADATA_SIZE;

    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE;

    address_list = A;

    metadata_t *found = find_right(A);
    // Tests
    cr_assert(zero(ptr, found), "Did not expect block after A");
}
Test(test_find_right, null_next_pointer) {
    // Setup
    C = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF + B_START_SIZE);
    C->size = C_START_SIZE - TOTAL_METADATA_SIZE;
    C->next = NULL;

    // 'B' is the freed block, but its next pointer shouldn't be relevant for
    // finding the immediate right block (set to NULL for this test)
    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;
    B->next = NULL;

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->next = C;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE;

    address_list = A;

    metadata_t *found = find_right(B);
    // Tests
    cr_assert(eq(ptr, found, C), "Expected correct block");
    cr_assert(eq(int, found->size, C->size), "Expected correct block size");
    cr_assert(eq(ptr, found->next, C->next), "Expected correct block next");
}