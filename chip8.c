#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "chip8.h"

const uint8_t chip8_font_set[FONT_SET_SIZE] = {
    0xF0,0x90,0x90,0x90,0xF0, // 0
    0x20,0x60,0x20,0x20,0x70, // 1
    0xF0,0x10,0xF0,0x80,0xF0, // 2
    0xF0,0x10,0xF0,0x10,0xF0, // 3
    0x90,0x90,0xF0,0x10,0x10, // 4
    0xF0,0x80,0xF0,0x10,0xF0, // 5
    0xF0,0x80,0xF0,0x90,0xF0, // 6
    0xF0,0x10,0x20,0x40,0x40, // 7
    0xF0,0x90,0xF0,0x90,0xF0, // 8
    0xF0,0x90,0xF0,0x10,0xF0, // 9
    0xF0,0x90,0xF0,0x90,0x90, // A
    0xE0,0x90,0xE0,0x90,0xE0, // B
    0xF0,0x80,0x80,0x80,0xF0, // C
    0xE0,0x90,0x90,0x90,0xE0, // D
    0xF0,0x80,0xF0,0x80,0xF0, // E
    0xF0,0x80,0xF0,0x80,0x80, // F
};

bool chip8_load_rom(p_chip8_t chip8, const char *path)
{
    FILE *f;
    f = fopen(path, "rb");
    if (!f) {
        perror("Failed to open ROM");
        return false;
    }

    fseek(f, 0, SEEK_END);
    long rom_size = ftell(f);
    printf("rom_size: %ld\n", rom_size);
    fseek(f, 0, SEEK_SET);

    return true;
}

void chip8_disassemble(const char *path) // change to emulate_cycle
{
    // decode opcodes
}

void chip8_init(p_chip8_t chip8)
{
    // init chip8 internals here
    //mem[4096];
    memset(chip8->mem, 0, 4096); // check for performance with for loop
    memcpy(chip8->mem, chip8_font_set, FONT_SET_SIZE);
    // V[16]; // Vx registers (x = 0 to F)
    // I;
    chip8->PC = 0x200;

    //stack[16];
    //SP;

    // 60Hz
    chip8->DT = 0; // delay timer
    chip8->ST = 0; // sound timer
    
    //display[y * 64 + x]
    //display[64 * 32];
}

void chip8_draw_window(p_chip8_t chip8) {

}
