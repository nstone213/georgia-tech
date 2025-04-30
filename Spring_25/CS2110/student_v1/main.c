// Name: Nicholas Stone

#include "gba.h"
#include "images/puppy.h"
#include "images/background.h"

volatile u16 *videoBuffer = (u16 *)0x6000000;

typedef struct ImgData {
	u32 width;
	u32 height;
	const u16 *image;
	u32 row;
	u32 col;
} ImgData; 

extern const unsigned short puppy[IMAGE_WIDTH*IMAGE_HEIGHT];
unsigned short puppy_red[IMAGE_WIDTH*IMAGE_HEIGHT];
unsigned short puppy_green[IMAGE_WIDTH*IMAGE_HEIGHT];
unsigned short puppy_blue[IMAGE_WIDTH*IMAGE_HEIGHT];

u16 redFilter(u16 pixel) {
    return pixel & 0x001F;
}

u16 greenFilter(u16 pixel) {
    return pixel & 0x03E0;
}

u16 blueFilter(u16 pixel) {
    return pixel & 0x7C00;
}

void drawImage(u32 width, u32 height, const u16 *image, u32 row, u32 col) {
    for (u32 i = 0; i < height; i++) {
        const u16 *source = image + (i * width);
        volatile u16 *fin = videoBuffer + ((row + i) * SCREEN_WIDTH + col);
        DMA[3].src = source;
        DMA[3].dst = fin;
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
    }
}


int main(void) {
	REG_DISPCNT = MODE3 | BG2_ENABLE;	
	waitForVBlank();

	for (int i = 0; i < IMAGE_WIDTH*IMAGE_HEIGHT; i++) {
		puppy_red[i] = redFilter(puppy[i]);
		puppy_green[i] = greenFilter(puppy[i]);
		puppy_blue[i] = blueFilter(puppy[i]);
	}
	
	struct ImgData image_array[] = {
		{SCREEN_WIDTH, SCREEN_HEIGHT, background, 0, 0},
		{IMAGE_WIDTH, IMAGE_HEIGHT, puppy, 9, 31},
		{IMAGE_WIDTH, IMAGE_HEIGHT, puppy_red, 9, 140},
		{IMAGE_WIDTH, IMAGE_HEIGHT, puppy_green, 89, 31},
		{IMAGE_WIDTH, IMAGE_HEIGHT, puppy_blue, 89, 140}
	};
    	
	for (int i = 0; i < 5; i++) {
        drawImage(image_array[i].width, image_array[i].height, image_array[i].image, image_array[i].row, image_array[i].col);
    }

#ifndef AG_RUNNING
	while (1);
#else
	return 0;
#endif
}


void waitForVBlank(void) {
#ifndef AG_RUNNING
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
#endif
}