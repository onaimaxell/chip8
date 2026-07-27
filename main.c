#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "chip8.h"
#include "gui.h"

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
    const int height = 720;
    const int width = 1280;
    InitWindow(width, height, "Chip8 Testing");

    chip8_t chip8 = {0};
    chip8_init(&chip8);
    printf("chip8 initialized\n");
    chip8_load_rom(&chip8, argv[1]);
    printf("loaded rom: %s\n", argv[1]);

    // 700 cycles / sec
    double cycles = 500;
    double time_step = 1 / cycles; // duration of each cycle
    double cpu_accumulator = 0;

    // ClearBackground(GRAY);
    while (!WindowShouldClose()) {
        double dt = GetFrameTime();
        cpu_accumulator += dt;
        // update var here

        BeginDrawing();


        while (cpu_accumulator >= time_step) {
            chip8_step(&chip8);
            cpu_accumulator -= time_step;
            if (chip8.draw_flag) {
                chip8_draw_display(&chip8);
                chip8.draw_flag = 0;
            }
        }
        DrawText("Chip8 test window", 0, 0, 10, RED);
        EndDrawing();
    }
}

