#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define FONT_SET_SIZE 80
#define FONT_SET_ADDRESS 0x50

#define RAM_SIZE 0x1000
#define ROM_START_ADDRESS 0x200
#define MAX_ROM_SIZE (RAM_SIZE - ROM_START_ADDRESS)

#define msn(x) (((x) & 0xF000) >> 12) // for table index
#define nnn(x) ((x) & 0x0FFF)   // addr
#define nn(x) ((x) & 0x00FF)    // kk(x) for some. (lowest 8 bit of the instruction)
#define n(x) ((x) & 0x000F)     // nibble. 4 bit constant)
#define x(x) (((x) & 0x0F00) >> 8)
#define y(x) (((x) & 0x00F0) >> 4)


// specs
typedef struct chip8_t {
    uint8_t mem[RAM_SIZE];
    uint8_t V[16]; // Vx registers (x = 0 to F)
    uint16_t I;
    uint16_t PC;
    uint16_t opcode;

    uint16_t stack[16];
    uint8_t SP;

    // 60Hz
    uint8_t DT; // delay timer
    uint8_t ST; // sound timer
    
    // (y * 64) + x
    uint8_t display[64 * 32];

    // keypad
    uint8_t key[16];
    bool key_pressed;

    bool draw_flag;
} chip8_t, *p_chip8_t;

typedef void (*op_table)(p_chip8_t chip8);
extern op_table table[16]; // dispatcher
extern const uint8_t chip8_font_set[FONT_SET_SIZE];

// opcode handlers
void op_0(p_chip8_t chip8);
void op_1(p_chip8_t chip8);
void op_2(p_chip8_t chip8);
void op_3(p_chip8_t chip8);
void op_4(p_chip8_t chip8);
void op_5(p_chip8_t chip8);
void op_6(p_chip8_t chip8);
void op_7(p_chip8_t chip8);
void op_8(p_chip8_t chip8);
void op_9(p_chip8_t chip8);
void op_A(p_chip8_t chip8);
void op_B(p_chip8_t chip8);
void op_C(p_chip8_t chip8);
void op_D(p_chip8_t chip8);
void op_E(p_chip8_t chip8);
void op_F(p_chip8_t chip8);

void chip8_draw(p_chip8_t chip8, uint8_t x, uint8_t y, uint8_t n);
bool chip8_load_rom(p_chip8_t chip8, const char *path);
void chip8_step(p_chip8_t chip8); // change to emulate_cycle
void chip8_init(p_chip8_t chip8);

#endif
