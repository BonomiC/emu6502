#include "cpu.h"
#include <stdio.h>

m6502Context context;
uint8_t mainMemory[MEMORY_SIZE];
m6502Instruction instruction_vector[MAX_INSTRUCTION_SIZE];

void initialize(void)
{
    memset(&context, 0, sizeof(m6502Context));
    memset(mainMemory, 0, 64 * 1024);

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

    // Execute the instruction
    instruction->exec(instruction, operand);
}

void adc(m6502Instruction *instruction, m6502Word operand)
{
    uint8_t val;
    switch (instruction->addrMode)
    {
    case IMMEDIATE:
        val = operand.l;
        break;
    case ZERO_PAGE:
        val = context.memory[operand.l];
        break;
    case ABSOLUTE:
        val = context.memory[operand.w];
        break;
    default:
        break;
    }

    m6502Word res = {.w=context.a + val + context.sr.c};
    context.sr.c = res.w > 0xFF;
    context.sr.z = res.w == 0;
    context.sr.v = CHECK_BIT((res.w ^ context.a) & (res.w ^ operand.w), 7);
    context.sr.n = CHECK_BIT(res.w, 7);
    context.a = res.l;
}

void lda(m6502Instruction *instruction, m6502Word operand)
{
    switch (instruction->addrMode)
    {
        case IMPLIED:
            break;
        case IMMEDIATE:
            context.a = operand.l;
            break;
        case ABSOLUTE:
            context.a = context.memory[operand.w];
            break;
    }
}

m6502Instruction instructions[256] =
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
    {&lda, 0xAD, ABSOLUTE, 3, 4}
};
