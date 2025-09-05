#include "cpu.h"
#include <stdio.h>

m6502Context context;
uint8_t mainMemory[MEMORY_SIZE];
m6502Instruction instruction_vector[MAX_INSTRUCTION_SIZE];

void initialize(void)
{
    memset(&context, 0, sizeof(m6502Context));
    memset(mainMemory, 0, MEMORY_SIZE);

    context.memory = mainMemory;

    build_instruction_vector();
}

void build_instruction_vector(void)
{
    for (uint16_t i; i < MAX_INSTRUCTION_SIZE; i++)
    {
        m6502Instruction instr = instructions[i];
        instruction_vector[instr.opcode] = instr;
    }
}

void print_context(void)
{
    printf("Context:\n");
    printf("\tPC: %d\n", context.pc);
    printf("\tA: %d\n", context.a);
    printf("\tX: %d\n", context.x);
    printf("\tY: %d\n", context.y);
}

void step(void)
{
    // Get the next opcode from memory
    uint8_t opcode = mainMemory[context.pc++];

    // Grab the correct instruction for the opcode
    m6502Instruction *instruction = &instruction_vector[opcode];

    // Extract any operand
    m6502Word operand;
    memcpy(&operand.w, &context.memory[context.pc], instruction->size-1);
    context.pc += instruction->size-1;

    // Get value based on address mode
    uint16_t value;
    switch (instruction->addrMode)
    {
    case IMPLIED:
        value = 0;
        break;
    case IMMEDIATE:
        value = operand.l;
        break;
    case ZERO_PAGE:
        value = context.memory[operand.l];
        break;
    case ZERO_PAGE_X:
        value = context.memory[(uint16_t)operand.l + context.x];
        break;
    case ZERO_PAGE_Y:
        value = context.memory[(uint16_t)operand.l + context.y];
        break;
    case ABSOLUTE:
        value = context.memory[operand.w];
        break;
    case ABSOLUTE_X:
        value = context.memory[operand.w + context.x];
        break;
    case ABSOLUTE_Y:
        value = context.memory[operand.w + context.y];
        break;
    case INDIRECT:
    {
        uint16_t addr;
        memcpy(&addr, context.memory + operand.w, sizeof(addr));
        value = context.memory[addr];
        break;
    }
    case INDIRECT_X:
    {
        uint8_t addr = context.memory[(uint16_t)operand.l + context.x];
        value = context.memory[addr];
        break;
    }
    case INDIRECT_Y:
    {
        uint16_t addr = context.memory[operand.l];
        addr += context.y;
        value = context.memory[addr];
        break;
    }
    case RELATIVE:
        value = context.pc + operand.w;
        break;
    }

    // Execute the instruction
    instruction->exec(instruction, value);
}

m6502Instruction instructions[MAX_INSTRUCTION_SIZE] =
{
    {&adc, 0x69, IMMEDIATE, 2, 2},
    {&adc, 0x65, ZERO_PAGE, 2, 3},
    {&adc, 0x75, ZERO_PAGE_X, 2, 4},
    {&adc, 0x6D, ABSOLUTE, 3, 4},
    {&adc, 0x7D, ABSOLUTE_X, 3, 4},
    {&adc, 0x79, ABSOLUTE_Y, 3, 4},
    {&adc, 0x61, INDIRECT_X, 2, 6},
    {&adc, 0x71, INDIRECT_Y, 2, 5},

    {&lda, 0xA9, IMMEDIATE, 2, 2},
    {&lda, 0xA5, ZERO_PAGE, 2, 3},
    {&lda, 0xB5, ZERO_PAGE_X, 2, 4},
    {&lda, 0xAD, ABSOLUTE, 3, 4},
    {&lda, 0xBD, ABSOLUTE_X, 3, 4},
    {&lda, 0xB9, ABSOLUTE_Y, 3, 4},
    {&lda, 0xA1, INDIRECT_X, 2, 6},
    {&lda, 0xB1, INDIRECT_Y, 2, 5},

    {&ldx, 0xA2, IMMEDIATE, 2, 2},
    {&ldx, 0xA6, ZERO_PAGE, 2, 3},
    {&ldx, 0xB6, ZERO_PAGE_Y, 2, 4},
    {&ldx, 0xAE, ABSOLUTE, 3, 4},
    {&ldx, 0xBE, ABSOLUTE_Y, 3, 4},

    {&ldy, 0xA0, IMMEDIATE, 2, 2},
    {&ldy, 0xA4, ZERO_PAGE, 2, 3},
    {&ldy, 0xB4, ZERO_PAGE_X, 2, 4},
    {&ldy, 0xAC, ABSOLUTE, 3, 4},
    {&ldy, 0xBC, ABSOLUTE_X, 3, 4}
};
