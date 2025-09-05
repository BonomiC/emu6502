#include <stdio.h>
#include "cpu.h"

int main(int argc, char** argv)
{
    initialize();

    uint8_t one[] = {0xA9, 27}; // LDA #27
    mainMemory[0] = 0xA9;
    mainMemory[1] = 27;
    memcpy(mainMemory, one, 2);

    uint8_t two[] = {0xAD, 0xCD, 0xAB}; // LDA $ABCD
    memcpy(mainMemory+2, two, 3);

    mainMemory[0xABCD] = 195;

    print_context();
    step();
    print_context();
    step();
    print_context();

    return 0;
}

