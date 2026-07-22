#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <raylib.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: ./main <rom>\n");
        exit(1);
    }
    // uint16_t x;
    // uint16_t *my_short = &x;
    // printf("nnn: %d\n", nnn(*my_short));
    // printf("bruh: nnn -> %x\n", *my_short & 0x0FFF);
    // printf("&x: %p\n", &x);
    // return 0;
    const int height = 600;
    const int width = 860;
    InitWindow(width, height, "Chip8 Testing");

    chip8_t chip8 = {0};
    chip8_init(&chip8);
    chip8_load_rom(&chip8, argv[1]);
    
    while (!WindowShouldClose()) {
        // update var here
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("Chip8 test window", 0, 0, 10, RED);
        EndDrawing();
    }
}

