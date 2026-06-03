#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define nnn(x) ((x) & 0x0FFF)   // addr
#define nn(x) ((x) & 0x00FF)    // kk(x) for some. (lowest 8 bit of the instruction)
#define n(x) ((x) & 0x000F)     // nibble. lowest 4 bit of the instruction)
#define x(x) (((x) & 0x0F00) >> 8)
#define y(x) (((x) & 0x00F0) >> 4)

// specs
struct chip8_t {
    uint8_t mem[4096];
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
};

#endif
