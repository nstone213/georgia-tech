#ifndef DMA_REGIONS_H
#define DMA_REGIONS_H

#include "dma_framework.h"

typedef struct {
    unsigned int original_index;
    unsigned int src_start;
    unsigned int src_end;
    unsigned int dst_start;
    unsigned int dst_end;
} dma_region_t;

void dma_regions_from_transfers(int chan, dma_transfer_t *transfer, int src_width, int src_height, const u16 *src, dma_region_t **arr_out, int *len_out);
void dma_regions_assert(dma_region_t *actual, int actual_len, dma_region_t *expected, int expected_len);

#endif
