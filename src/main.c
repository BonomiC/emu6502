#include <stdio.h>
#include "cpu.h"

int main(int argc, char** argv)
{
    initialize();

    uint16_t ram_pos = 0;

    uint8_t one[] = {0xA9, 27}; // LDA #27
    mainMemory[0] = 0xA9;
    mainMemory[1] = 27;
    memcpy(mainMemory + ram_pos, one, 2);
    ram_pos += 2;

    uint8_t two[] = {0xAD, 0xCD, 0xAB}; // LDA $ABCD
    memcpy(mainMemory+ram_pos, two, 3);
    ram_pos += 3;

    uint8_t three[] = {0x69, 0xA}; // ADC #10
    memcpy(mainMemory+ram_pos, three, 2);
    ram_pos += 2;

    uint8_t four[] = {0x65, 0xBF}; // ADC $BF
    memcpy(mainMemory+ram_pos, four, 2);
    ram_pos += 2;

    mainMemory[0xABCD] = 195;
    mainMemory[0x00BF] = 20;

    print_context();
    step();
    print_context();
    step();
    print_context();
    step();
    print_context();
    step();
    print_context();

    return 0;
}

