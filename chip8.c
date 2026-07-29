#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

const uint8_t keymap[16] = {
    KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,
    KEY_Q, KEY_W, KEY_E, KEY_R,
    KEY_A, KEY_S, KEY_D, KEY_F,
    KEY_Z, KEY_X, KEY_C, KEY_V,
};

void op_0(p_chip8_t chip8)
{
    switch(nn(chip8->opcode)) {
        case 0x00E0:
            printf("disp_clear()\n");
            memset(chip8->display, 0, sizeof(chip8->display));
            chip8->draw_flag = 1;
            break;
        case 0x00EE:
            chip8->PC = chip8_pop_stack(chip8);
            printf("return\n"); break;
        default:
            printf("invalid 0xxx opcode\n");
    }
}

void op_1(p_chip8_t chip8) {
    uint16_t addr = nnn(chip8->opcode);
    chip8->PC = addr;
    printf("goto nnn[%hx]\n", addr);
}

void op_2(p_chip8_t chip8)
{
    uint16_t addr = nnn(chip8->opcode);
    chip8_push_stack(chip8);
    chip8->PC = addr;
    printf("call nnn[%hx]\n", addr);
}

void op_3(p_chip8_t chip8) // Vx == nn
{
    uint8_t x = x(chip8->opcode);
    uint8_t nn = nn(chip8->opcode);
    if (chip8->V[x] == nn)
        chip8->PC += 2;
    printf("if (Vx[%hhx] == nn[%hhx])\n", x, nn);
}

void op_4(p_chip8_t chip8) // Vx != nn
{
    uint8_t x = x(chip8->opcode);
    uint8_t nn = nn(chip8->opcode);
    if (chip8->V[x] != nn)
        chip8->PC += 2;
    printf("if (Vx[%hhx] != nn[%hhx])\n", x, nn);
}

void op_5(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    uint8_t y = y(chip8->opcode);
    if (chip8->V[x] == chip8->V[y])
        chip8->PC += 2;
    printf("if (Vx[%hhx] == Vy[%hhx])\n", x, y);
}

void op_6(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    uint8_t nn = nn(chip8->opcode);
    chip8->V[x] = nn;
    printf("Vx[%hhx] = nn[%hhx]\n", x, nn);
}

void op_7(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    uint8_t nn = nn(chip8->opcode);
    chip8->V[x] += nn;
    printf("Vx[%hhx] += nn[%hhx]\n", x, nn);
}

void op_8(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    uint8_t y = y(chip8->opcode);

    switch (n(chip8->opcode)) {
        case 0x0:
            chip8->V[x] = chip8->V[y];
            printf("Vx[%hhx] = Vy[%hhx]\n", x, y);
            break;
        case 0x1:
            chip8->V[x] |= chip8->V[y];
            printf("Vx[%hhx] |= Vy[%hhx]\n", x, y);
            break;
        case 0x2:
            chip8->V[x] &= chip8->V[y];
            printf("Vx[%hhx] &= Vy[%hhx]\n", x, y);
            break;
        case 0x3:
            chip8->V[x] ^= chip8->V[y];
            printf("Vx[%hhx] ^= Vy[%hhx]\n", x, y);
            break;
        case 0x4:
            // handle VF overflow
            chip8->V[x] += chip8->V[y];
            printf("Vx[%hhx] += Vy[%hhx]\n", x, y);
            break;
        case 0x5:
            chip8->V[x] -= chip8->V[y];
            printf("Vx[%hhx] -= Vy[%hhx]\n", x, y);
            break;
        case 0x6:
            // make this configurable
            chip8->V[x] >>= 1;
            printf("Vx[%hhx] >>= 1", x);
            break;
        case 0x7:
            // handle VF overflow
            chip8->V[x] = chip8->V[y] - chip8->V[x];
            printf("Vx[%hhx] = Vy[%hhx] - Vx[%hhx]\n", x, y, x);
            break;
        case 0xE:
            // make this configurable
            chip8->V[x] <<= 1;
            printf("Vx[%hhx] <<= 1", x);
            break;
        default: printf("invalid 8xy0 opcode\n");
    }
}

void op_9(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    uint8_t y = y(chip8->opcode);
    if (chip8->V[x] != chip8->V[y])
        chip8->PC +=2;
    printf("if (Vx[%hhx] != Vy[%hhx])\n", x, y);
}

void op_A(p_chip8_t chip8)
{
    uint16_t addr = nnn(chip8->opcode);
    chip8->I = addr;
    printf("I = nnn[%hx]\n", addr);
}

// make this configurable
void op_B(p_chip8_t chip8)
{
    uint16_t addr = nnn(chip8->opcode);
    chip8->PC = addr + chip8->V[0];
    printf("PC[%hx] = V0[%hhx] + nnn[%hx]\n", chip8->PC, chip8->V[0], addr);
}

void op_C(p_chip8_t chip8) // rand()
{
    uint8_t x = x(chip8->opcode);
    uint8_t nn = nn(chip8->opcode);
    chip8->V[x] = (rand() % 255) & nn;
    printf("Vx[%hhx] = rand() & nn[%hhx]\n", x, nn);
}

void op_D(p_chip8_t chip8) // draw()
{
    uint8_t x = x(chip8->opcode);
    uint8_t y = y(chip8->opcode);
    uint8_t n = n(chip8->opcode);
    chip8_draw(chip8, chip8->V[x] % DISPLAY_WIDTH, chip8->V[y] % DISPLAY_HEIGHT, n);
    printf("draw(Vx[%hhx], Vy[%hhx], nn[%hhx])\n", x, y, n);
}

void op_E(p_chip8_t chip8) // key()
{
    uint8_t x = x(chip8->opcode);
    switch (nn(chip8->opcode)) {
        case 0x9E:
            if (chip8->key[(chip8->V[x] & 0x0F)])
                chip8->PC += 2;
            printf("if (key() == Vx[%hhx]\n", x);
            break;
        case 0xA1:
            if (!chip8->key[(chip8->V[x] & 0x0F)])
                chip8->PC +=2;
            printf("if (key() != Vx[%hhx]\n", x);
            break;
        default:
            printf("invalid Ex00 opcode\n");
    }
}

void op_F(p_chip8_t chip8)
{
    uint8_t x = x(chip8->opcode);
    switch (nn(chip8->opcode)) {
        case 0x07:
            chip8->V[x] = chip8->DT;
            printf("Vx[%hhx] = get_delay()\n", x);
            break; // get_delay()
        case 0x0A:
            // i think this is a bug lol
            // debug this shit, make sure key_pressed array is working correctly
            for (int i = 0; i < KEYPAD_SIZE; i++) {
                if (!chip8->key[i] && chip8->key_pressed[i]) {
                    chip8->V[x] = i;
                    printf("Vx[%hhx] = get_key()\n", x);
                    break;
                }
            }
            chip8->PC -= 2;
            break; // get_key()
        case 0x15:
            chip8->DT = chip8->V[x];
            printf("DT = Vx[%hhx]\n", x);
            break; // delay_timer = Vx
        case 0x18:
            chip8->ST = chip8->V[x];
            printf("ST = Vx[%hhx]\n", x);
            break; // sound_timer = Vx
        case 0x1E:
            chip8->I += chip8->V[x];
            printf("I += Vx[%hhx]\n", x);
            break; // I += Vx
        case 0x29:
            // this could bug lol
            chip8->I = FONT_SET_ADDRESS + ((chip8->V[x] & 0x0F) * 5);
            printf("I = sprite_addr[Vx[%hhx]]\n", x);
            break; 
        case 0x33:
            chip8->mem[chip8->I + 2] = chip8->V[x] % 10;            // ones
            chip8->mem[chip8->I + 1] = (chip8->V[x] % 100) / 10;    // tens
            chip8->mem[chip8->I]     = chip8->V[x] / 100;           // hundreds
            printf("BCD stuff\n");
            break; // BCD
        case 0x55:
            // check if correct
            for (int i = 0; i <= x; i++) {
                chip8->mem[chip8->I + i] = chip8->V[i];
            }
            printf("reg_dump(Vx[%hhx], &I\n", x);
            break;
        case 0x65:
            // check if correct
            for (int i = 0; i <= x; i++) {
                chip8->V[i] = chip8->mem[chip8->I + i];
            }
            printf("reg_load(Vx[%hhx], &I\n", x);
            break;
        default: printf("invalid FX00 opcode\n");
    }
}

op_table table[16] = {
    op_0, op_1, op_2, op_3,
    op_4, op_5, op_6, op_7,
    op_8, op_9, op_A, op_B,
    op_C, op_D, op_E, op_F,
};

void chip8_draw(p_chip8_t chip8, uint8_t x, uint8_t y, uint8_t sprite_height)
{
    // basically we loop through each bit horizontally (msb -> lsb, 8 times), then proceed to next byte (height)
    chip8->V[0xF] = 0;
    for (int n = 0; n < sprite_height; n++) {
        uint8_t pixel = chip8->mem[chip8->I + n];
        for (int bit_col = 0; bit_col < 8; bit_col++) {       // bit_col = 1 byte / 8 bit
            // 0xF0 == 0b10000000 >> bit_col -> bit_col mov from msb to lsb per iteration
            if ((pixel & (0x80 >> bit_col)) != 0) {
                if (chip8->display[((y + n) * DISPLAY_WIDTH) + x + bit_col] == 1) // if collision
                    chip8->V[0xF] = 1;
                chip8->display[((y + n) * DISPLAY_WIDTH) + x + bit_col] ^= 1; // flip the pixel
            }
        }
    }
    chip8->draw_flag = 1;
}

bool chip8_load_rom(p_chip8_t chip8, const char *path)
{
    FILE *f;
    bool retval = 1;

    if (!chip8 || !path) {
        perror("passing argument null to chip8_load_rom");
        return 1;
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
        perror("ROM size too big to fit in memory");
        goto cleanup;
    }

    // for loading new ROM during execution
    // memset(chip8->mem + ROM_START_ADDRESS, 0, MAX_ROM_SIZE);

    size_t rbytes= fread(chip8->mem + ROM_START_ADDRESS, 1, rom_size, f);
    if (rbytes != (size_t) rom_size) {
        perror("Error reading ROM: Invalid size");
        goto cleanup;
    }

    retval = 0;

cleanup:
    if (f) fclose(f);
    return retval;
}

void chip8_step(p_chip8_t chip8)
{
    // fetch/decode opcodes
    chip8->opcode = (chip8->mem[chip8->PC] << 8) | chip8->mem[chip8->PC + 1];
    printf("%hx\t%hx\t --> ", chip8->PC, chip8->opcode);
    //
    if (chip8->PC > MAX_ROM_SIZE) exit(66);
    //
    chip8->PC += 2;
    //execute
    table[msn(chip8->opcode)](chip8);
}

void chip8_init(p_chip8_t chip8)
{
    // init chip8 internals here
    memset(chip8->mem, 0, 4096); // check for performance with for loop
    memset(chip8->V, 0, sizeof(chip8->V));
    chip8->I = 0;
    chip8->PC = ROM_START_ADDRESS;

    //stack[16];
    memset(chip8->stack, 0, sizeof(chip8->stack));
    chip8->SP = -1;

    // 60Hz
    chip8->DT = 0; // delay timer
    chip8->ST = 0; // sound timer
    
    memset(chip8->key, 0, sizeof(chip8->key));
    memset(chip8->key_pressed, 0, sizeof(chip8->key_pressed));
    memset(chip8->display, 0, sizeof(chip8->display));
    memcpy(chip8->mem + FONT_SET_ADDRESS, chip8_font_set, FONT_SET_SIZE); // load font set at 0x50
    chip8->draw_flag = false;
    srand(time(NULL));  // seed the rng
}

void chip8_push_stack(p_chip8_t chip8)
{
    if (chip8->SP >= (int8_t) (sizeof(chip8->stack) / sizeof(chip8->stack[0]))) {
        printf("error: stack overflow\n");
        exit(1);
    }

    chip8->stack[++chip8->SP] = chip8->PC;
}
uint16_t chip8_pop_stack(p_chip8_t chip8)
{
    if (chip8->SP < 0) {
        printf("error: stack is already empty\n");
        exit(2);
    }

    return chip8->stack[chip8->SP--];
}

void get_key(p_chip8_t chip8)
{
    int key = GetKeyPressed();

    // TODO quit, reload, load, pause, single-step, breakpoint at current selected line/clicked

    // set key
    for (int i = 0; i < KEYPAD_SIZE; i++) {
        if (key == keymap[i]) {
            chip8->key[i] = 1;
            chip8->key_pressed[i] = 1;
            break; // detect only the first key (idk if this is right) or it just make it a bit efficient idk
        }
    }

    // unset key
    for (int i = 0; i < KEYPAD_SIZE; i++) {
        if (IsKeyUp(keymap[i])) {
            chip8->key[i] = 0;
        }
    }
}

