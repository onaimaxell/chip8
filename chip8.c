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
    bool success = false;

    if (!chip8 || !path) {
        perror("passing argument null to chip8_load_rom");
        return false;
    }

    f = fopen(path, "rb");
    if (!f) {
        perror("Failed to open ROM");
        goto cleanup;
    }

    fseek(f, 0, SEEK_END);
    long rom_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (rom_size > MAX_ROM_SIZE) {
        perror("ROM size loaded is bigger than 3.5kb");
        goto cleanup;
    }

    size_t rbytes= fread(chip8->mem + ROM_START_ADDRESS, 1, rom_size, f);
    if (rbytes != rom_size) {
        perror("Error reading ROM: Invalid size");
        goto cleanup;
    }

    success = true;

cleanup:
    if (f) {
        fclose(f);
    }
    return success;
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
    memcpy(chip8->mem + FONT_SET_ADDRESS, chip8_font_set, FONT_SET_SIZE);
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
