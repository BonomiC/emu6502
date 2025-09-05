#include <stdio.h>
#include "cpu.h"

#define ADD_INSTRUCTION(...) add_instruction((uint8_t[]){__VA_ARGS__}, sizeof((uint8_t[]){__VA_ARGS__}))

void add_instruction(uint8_t *instructions, uint8_t size)
{
    static uint16_t ram_pos = 1;
    memcpy(mainMemory + ram_pos, instructions, size);
    ram_pos += size;
}

int main(int argc, char** argv)
{
    initialize();

    context.pc = 0x0001;

    ADD_INSTRUCTION(0xA9, 27); // LDA #27

    ADD_INSTRUCTION(0xAD, 0xCD, 0xAB); // LDA $ABCD

    ADD_INSTRUCTION(0x69, 0xA); // ADC #10

    ADD_INSTRUCTION(0xA2, 0x10); // LDX #10

    ADD_INSTRUCTION(0x65, 0xBF); // ADC $BF

    ADD_INSTRUCTION(0x75, 0xAF); // ADC $AF,X

    ADD_INSTRUCTION(0x0A); // ASL

    ADD_INSTRUCTION(0x06, 0xCD); // ASL $CD

    ADD_INSTRUCTION(0xA4, 0xCD); // LDY $CD

    // ADD_INSTRUCTION(0x90, -6); // BCC -6 // Will cause loop prev 2 instructions until carry bit is set

    mainMemory[0xABCD] = 195;
    mainMemory[0x00BF] = 20;
    mainMemory[0x00CD] = 17;

    print_context();

    while (mainMemory[context.pc] != 0x00)
    {
        step();
        print_context();
    }

    return 0;
}

