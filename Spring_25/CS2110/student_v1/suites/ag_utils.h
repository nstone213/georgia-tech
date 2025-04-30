#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "../gba.h"
#include "dma_framework.h"
#include "dma_regions.h"
#include <stdlib.h>
#include <string.h>

// The Makefile script + Criterion bugs are causing weird interactions.
// `make run-tests` cannot be ^C'd (it has to be ^Z'd).
//
// As such, we need to impose a timeout so that tests will eventually stop
// in cases of infinite loops.
//
// However, we do not want to impose a timeout when debugging with GDB
// (since that wouldn't be a fun debugging experience).
//
// One way of doing this is defining ./tests --timeout N in make run-tests;
// however, this requires specifying a non-default timeout (because Criterion bug).
// You can't set +INFINITY because +INFINITY (also due to a bug) acts as a 0s timeout.
#define UNREASONABLY_LONG 86400

extern volatile unsigned short *_reg_dispcnt;
extern volatile DMA_CONTROLLER *_dma;

// Used for special printing in AG
typedef u16 Color;

// redefine ImgData b/c I don't want to move ImgData outside of main.c
typedef struct ImgData {
    u32 width;
    u32 height;
    const u16 *image;
    u32 row;
    u32 col;
} ImgData;

// Below are autograder helper functions, you may NOT use these in your solution
u16 h_red_filter(u16 pixel);
u16 h_green_filter(u16 pixel);
u16 h_blue_filter(u16 pixel);

char *cr_user_Color_tostr(Color *value);
int cr_user_Color_eq(Color *a, Color *b);

dma_region_t *h_draw_dma_calls(u32 width, u32 height, u32 row, u32 col);
void h_draw_image(u16 *dest, u32 width, u32 height, const u16 *image, u32 row, u32 col);
void h_simulate_main(u16 *buffer, struct ImgData pictures[]);
#endif
