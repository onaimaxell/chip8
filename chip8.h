#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define FONT_SET_SIZE 80
#define FONT_SET_ADDRESS 0x50
#define RAM_SIZE 0x1000
#define ROM_START_ADDRESS 0x200
#define MAX_ROM_SIZE (RAM_SIZE - ROM_START_ADDRESS)

#define nnn(x) ((x) & 0x0FFF)   // addr
#define nn(x) ((x) & 0x00FF)    // kk(x) for some. (lowest 8 bit of the instruction)
#define n(x) ((x) & 0x000F)     // nibble. lowest 4 bit of the instruction)
#define x(x) (((x) & 0x0F00) >> 8)
#define y(x) (((x) & 0x00F0) >> 4)

// specs
typedef struct chip8_t {
    uint8_t mem[RAM_SIZE];
    uint8_t V[16]; // Vx registers (x = 0 to F)
    uint16_t I;
    uint16_t PC;

    uint16_t stack[16];
    uint8_t SP;

    // 60Hz
    uint8_t DT; // delay timer
    uint8_t ST; // sound timer
    
    //display[y * 64 + x]
    uint8_t display[64 * 32];
} chip8_t, *p_chip8_t;

extern const uint8_t chip8_font_set[FONT_SET_SIZE];

bool chip8_load_rom(p_chip8_t chip8, const char *path);
void chip8_disassemble(const char *path); // change to emulate_cycle
void chip8_init(p_chip8_t chip8);
void chip8_draw_window(p_chip8_t chip8);

#endif
