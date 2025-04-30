#include "../images/background.h"
#include "../images/puppy.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

volatile unsigned short *_reg_dispcnt;
volatile DMA_CONTROLLER *_dma;

static u16 *h_video_buffer;
static void setup_gba(void) {
    videoBuffer = calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u16));
    h_video_buffer = calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u16));
    _reg_dispcnt = calloc(1, sizeof(u16));

    dma_setup(DMA_SIM_CHAN_3, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(u16));
}
static void teardown_gba(void) {
    free((void *)videoBuffer);
    videoBuffer = NULL;

    free(h_video_buffer);
    h_video_buffer = NULL;

    free((void *)_reg_dispcnt);
    _reg_dispcnt = NULL;

    dma_teardown();
}

TestSuite(test_filters, .timeout = UNREASONABLY_LONG);

Test(test_filters, red_0) {
    u16 in = 0x7FFF; // white
    u16 expected = h_red_filter(in);
    u16 actual = redFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, red_1) {
    u16 in = 0x001F; // max red
    u16 expected = h_red_filter(in);
    u16 actual = redFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, red_2) {
    u16 in = 0x03E0; // green only
    u16 expected = h_red_filter(in);
    u16 actual = redFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, red_3) {
    u16 in = 0x7C00; // blue only
    u16 expected = h_red_filter(in);
    u16 actual = redFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, red_4) {
    u16 in = 0x0000; // black
    u16 expected = h_red_filter(in);
    u16 actual = redFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, green_0) {
    u16 in = 0x7FFF; // white
    u16 expected = h_green_filter(in);
    u16 actual = greenFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, green_1) {
    u16 in = 0x03E0; // max green
    u16 expected = h_green_filter(in);
    u16 actual = greenFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, green_2) {
    u16 in = 0x001F; // red only
    u16 expected = h_green_filter(in);
    u16 actual = greenFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, green_3) {
    u16 in = 0x7C00; // blue only
    u16 expected = h_green_filter(in);
    u16 actual = greenFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, green_4) {
    u16 in = 0x0000; // black
    u16 expected = h_green_filter(in);
    u16 actual = greenFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, blue_0) {
    u16 in = 0x7FFF; // white
    u16 expected = h_blue_filter(in);
    u16 actual = blueFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, blue_1) {
    u16 in = 0x7C00; // max blue
    u16 expected = h_blue_filter(in);
    u16 actual = blueFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, blue_2) {
    u16 in = 0x001F; // red only
    u16 expected = h_blue_filter(in);
    u16 actual = blueFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, blue_3) {
    u16 in = 0x03E0; // green only
    u16 expected = h_blue_filter(in);
    u16 actual = blueFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, blue_4) {
    u16 in = 0x0000; // black
    u16 expected = h_blue_filter(in);
    u16 actual = blueFilter(in);
    cr_assert(eq(type(Color), actual, expected));
}
Test(test_filters, comprehensive) {
    for (u16 i = 0; i <= 0x7FFF; i++) {
        cr_assert(eq(type(Color), redFilter(i), h_red_filter(i)), "redFilter failed at 0x%04x", i);
        cr_assert(eq(type(Color), greenFilter(i), h_green_filter(i)), "greenFilter failed at 0x%04x", i);
        cr_assert(eq(type(Color), blueFilter(i), h_blue_filter(i)), "blueFilter failed at 0x%04x", i);
    }
}

TestSuite(test_drawImage, .timeout = UNREASONABLY_LONG, .init = setup_gba, .fini = teardown_gba);

Test(test_drawImage, puppy) {
    h_draw_image(h_video_buffer, PUPPY_WIDTH, PUPPY_HEIGHT, puppy, 27, 29);
    drawImage(PUPPY_WIDTH, PUPPY_HEIGHT, puppy, 27, 29);
    dma_stop();

    // Verify that the student has correctly updated the video buffer / screen
    cr_assert(
        zero(int, memcmp((void *) videoBuffer, h_video_buffer, sizeof(u16) * SCREEN_WIDTH * SCREEN_HEIGHT)),
        "Expected image to have been properly drawn to the video buffer/display."
    );
}

static dma_region_t *actual_dma = NULL;
static dma_region_t *expected_dma = NULL;
static void teardown_drawImageDMA(void) {
    free(actual_dma);
    free(expected_dma);
}
Test(test_drawImage, puppy_dma, .fini = teardown_drawImageDMA) {
    drawImage(PUPPY_WIDTH, PUPPY_HEIGHT, puppy, 27, 29);
    dma_stop();

    int actual_len;
    dma_regions_from_transfers(3, dma_transfers[3], PUPPY_WIDTH, PUPPY_HEIGHT, puppy, &actual_dma, &actual_len);

    // Create expected:
    expected_dma = h_draw_dma_calls(PUPPY_WIDTH, PUPPY_HEIGHT, 27, 29);
    int expected_len = PUPPY_HEIGHT;

    dma_regions_assert(actual_dma, actual_len, expected_dma, expected_len);
}

// idk if you need more tests for this lol

TestSuite(test_main, .timeout = UNREASONABLY_LONG, .init = setup_gba, .fini = teardown_gba);
Test(test_main, correct) {
    // this is given to the student's within main
    const unsigned short expected_dispcnt_value = MODE3 | BG2_ENABLE;

    // Bring in images to draw
    unsigned short puppy_red[IMAGE_WIDTH * IMAGE_HEIGHT];
    unsigned short puppy_green[IMAGE_WIDTH * IMAGE_HEIGHT];
    unsigned short puppy_blue[IMAGE_WIDTH * IMAGE_HEIGHT];

    struct ImgData image_array[] = {
        {SCREEN_WIDTH, SCREEN_HEIGHT, background, 0, 0},
        {IMAGE_WIDTH, IMAGE_HEIGHT, puppy, 9, 31},
        {IMAGE_WIDTH, IMAGE_HEIGHT, puppy_red, 9, 140},
        {IMAGE_WIDTH, IMAGE_HEIGHT, puppy_green, 89, 31},
        {IMAGE_WIDTH, IMAGE_HEIGHT, puppy_blue, 89, 140}
    };

    // correctly filter the dog images
    for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
        puppy_red[i] = h_red_filter(puppy[i]);
        puppy_green[i] = h_green_filter(puppy[i]);
        puppy_blue[i] = h_blue_filter(puppy[i]);
    }

    // Perform the same actions onto the buffer that student solution would
    h_simulate_main(h_video_buffer, image_array);
    // call the student's main function
    student_main();
    dma_stop(); // Flush final DMA call

    // Verify that the student has not changed _reg_dispcnt
    struct cr_mem expected_dispcnt = {.data = &expected_dispcnt_value, .size = sizeof(u16)};
    struct cr_mem actual_dispcnt = {.data = (u16 *)_reg_dispcnt, .size = sizeof(u16)};

    cr_assert(eq(mem, actual_dispcnt, expected_dispcnt), "Solution changes the value of display control register.");

    // Verify that the student has correctly updated the video buffer / screen
    cr_assert(
        zero(int, memcmp((void *) videoBuffer, h_video_buffer, sizeof(u16) * SCREEN_WIDTH * SCREEN_HEIGHT)),
        "Expected all images to have been properly drawn to the video buffer/display."
    );
}
