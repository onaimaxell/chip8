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
    // force load
    chip8.mem[0x1FF] = 1;

    // 700 cycles / sec
    double cycles = 300;
    double cpu_timestep = 1 / cycles; // duration of each cycle
    double cpu_accumulator = 0;

    // timers
    double timer = 60;
    double timer_timestep = 1 / timer;
    double timer_accumulator = 0;

    // ClearBackground(GRAY);
    while (!WindowShouldClose()) {
        double dt = GetFrameTime();
        cpu_accumulator += dt;
        timer_accumulator += dt;
        get_key(&chip8);

        BeginDrawing();

        while (cpu_accumulator >= cpu_timestep) {
            chip8_step_cpu(&chip8);
            cpu_accumulator -= cpu_timestep;
        }

        while (timer_accumulator >= timer_timestep) {
            // chip8_step_timer(&chip8, dt);
            if (chip8.DT > 0) {
                chip8.DT--;
            }

            if (chip8.ST > 0) {
                chip8.ST--;
                printf("beep\n");
            }

            timer_accumulator -= timer_timestep;
        }

        if (chip8.ST > 0) {
                printf("beepppp\n");
        }

        if (chip8.draw_flag) {
            chip8_draw_display(&chip8);
            chip8.draw_flag = 0;
        }

        DrawText("Chip8 test window", 0, 0, 10, RED);
        EndDrawing();
    }
}

