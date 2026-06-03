#include <stdio.h>
#include "chip8.h"

int main(void)
{
    uint16_t x;
    uint16_t *my_short = &x;
    printf("nnn: %d\n", nnn(*my_short));
    printf("bruh: nnn -> %x\n", *my_short & 0x0FFF);
    printf("&x: %p\n", &x);
    return 0;
}

