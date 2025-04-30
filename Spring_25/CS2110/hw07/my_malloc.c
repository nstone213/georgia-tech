/*
 * CS 2110 Homework 7 Spring 2025
 * Author: Nicholas Stone
 */

#include <string.h>
#include <stdio.h>
#include "my_malloc.h"

metadata_t *address_list;

enum my_malloc_err my_malloc_errno;

metadata_t *find_right(metadata_t *freed_block) {
    metadata_t *curr = address_list;
    
    char *right_edge = (char *)freed_block + TOTAL_METADATA_SIZE + freed_block->size;
    
    while (curr != NULL) {
        if ((char *)curr == right_edge) {
            return curr;
        }
        curr = curr->next;
    }
    
    return NULL;
}

metadata_t *find_left(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr && ((char *)freed_block > (char *)curr)) {
        char *curr_right = ((char *)curr) + TOTAL_METADATA_SIZE + curr->size;
        if (curr_right == (char *)freed_block) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

void merge(metadata_t *left, metadata_t *right) {
    if (!left || !right) {
        return;
    }
    
    left->size += TOTAL_METADATA_SIZE + right->size;
    
    left->next = right->next;
}

metadata_t *split_block(metadata_t *block, size_t size) {
    if (!block) {
        return NULL;
    }
    
    size_t total_size = TOTAL_METADATA_SIZE + size;
    
    metadata_t *right_block = (metadata_t *)((char *)block + block->size + TOTAL_METADATA_SIZE - total_size);
    
    block->size = block->size - total_size;
    
    right_block->size = size;
    right_block->next = NULL;
    return right_block;
}

void add_to_addr_list(metadata_t *block) {
    if (!block) {
        return;
    }
    
    if (!address_list) {
        address_list = block;
        block->next = NULL;
        return;
    }
    
    if ((char *)block < (char *)address_list) {
        block->next = address_list;
        address_list = block;
        return;
    }
    
    metadata_t *curr = address_list;
    metadata_t *prev = NULL;
    
    while (curr && (char *)block > (char *)curr) {
        prev = curr;
        curr = curr->next;
    }
    
    prev->next = block;
    block->next = curr;
}

void remove_from_addr_list(metadata_t *block) {
    metadata_t *curr = address_list;
    if (!curr) {
        return;
    } else if (curr == block) {
        address_list = curr->next;
    }

    metadata_t *next;
    while ((next = curr->next) && (char *)block > (char *)next) {
        curr = next;
    }
    if (next == block) {
        curr->next = next->next;
    }
}

metadata_t *find_best_fit(size_t size) {
    if (!address_list) {
        return NULL;
    }
    
    metadata_t *curr = address_list;
    metadata_t *bestfit = NULL;
    
    while (curr) {
        if (curr->size == size) {
            return curr;
        }
        
        if (curr->size > size && (!bestfit || curr->size < bestfit->size)) {
            bestfit = curr;
        }
        
        curr = curr->next;
    }
    
    return bestfit;
}

void *my_malloc(size_t size) {
    my_malloc_errno = NO_ERROR;
    
    if (size == 0) {
        return NULL;
    }
    
    if (size > SBRK_SIZE - TOTAL_METADATA_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }
    
    metadata_t *block_best = find_best_fit(size);
    
    if (!block_best) {
        metadata_t *block_new = my_sbrk(SBRK_SIZE);
        
        if (block_new == (metadata_t *)-1) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }
        
        block_new->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
        block_new->next = NULL;
        
        add_to_addr_list(block_new);
        
        metadata_t *left = find_left(block_new);
        if (left) {
            remove_from_addr_list(left);
            remove_from_addr_list(block_new);
            merge(left, block_new);
            add_to_addr_list(left);
            block_new = left;
        }
        
        block_best = find_best_fit(size);
    }
    
    if (block_best->size < size + TOTAL_METADATA_SIZE + 1) {
        remove_from_addr_list(block_best);
    } else {
        metadata_t *right_block = split_block(block_best, size);
        return (void *)((char *)right_block + TOTAL_METADATA_SIZE);
    }
    
    return (void *)((char *)block_best + TOTAL_METADATA_SIZE);
}

void my_free(void *ptr) {
    my_malloc_errno = NO_ERROR;
    
    if (!ptr) {
        return;
    }
    
    metadata_t *block = (metadata_t *)((char *)ptr - TOTAL_METADATA_SIZE);
    
    add_to_addr_list(block);
    metadata_t *block_left = find_left(block);
    if (block_left) {
        remove_from_addr_list(block_left);
        remove_from_addr_list(block);
        merge(block_left, block);
        add_to_addr_list(block_left);
        block = block_left;
    }
    
    metadata_t *block_right = find_right(block);
    if (block_right) {
        remove_from_addr_list(block);
        remove_from_addr_list(block_right);
        merge(block, block_right);
        add_to_addr_list(block);
    }
}

void *my_realloc(void *ptr, size_t size) {
    my_malloc_errno = NO_ERROR;
    
    if (!ptr) {
        return my_malloc(size);
    }
    
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    metadata_t *block = (metadata_t *)((char *)ptr - TOTAL_METADATA_SIZE);
    
    void *new_ptr = my_malloc(size);
    
    if (!new_ptr) {
        return NULL;
    }
    
    size_t copy = (block->size < size) ? block->size : size;
    memcpy(new_ptr, ptr, copy);
    my_free(ptr);
    return new_ptr;
}

void *my_calloc(size_t nmemb, size_t size) {
    my_malloc_errno = NO_ERROR;
    
    size_t size_total;
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
    
    if (nmemb > SIZE_MAX / size) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }
    
    size_total = nmemb * size;
    void *ptr = my_malloc(size_total);
    
    if (ptr) {
        memset(ptr, 0, size_total);
    }
    
    return ptr;
}