#include <stdio.h>
#include <raylib.h>

#include "chip8.h"

#define PIXEL_HEIGHT 15
#define PIXEL_WIDTH 15
#define BORDER_SIZE 2

void chip8_draw_display(p_chip8_t chip8)
{
    // for (int i = 0; i < 64; i++) {
    //     chip8->display[(0 * DISPLAY_WIDTH) + i] = 1;
    // }
    for (int h = 0; h < DISPLAY_HEIGHT; h++) {
        for (int c = 0; c < DISPLAY_WIDTH; c++) {
            if (chip8->display[(h * DISPLAY_WIDTH) + c] == 1)
                DrawRectangle(BORDER_SIZE + 0 + (c * PIXEL_WIDTH), BORDER_SIZE + 0 + (h * PIXEL_HEIGHT), PIXEL_WIDTH - BORDER_SIZE, PIXEL_HEIGHT - BORDER_SIZE, WHITE);
            else
                DrawRectangle(BORDER_SIZE + 0 + (c * PIXEL_WIDTH), BORDER_SIZE + 0 + (h * PIXEL_HEIGHT), PIXEL_WIDTH - BORDER_SIZE, PIXEL_HEIGHT - BORDER_SIZE, BLACK);
        }
    }
}
