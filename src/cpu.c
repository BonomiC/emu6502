#include "cpu.h"
#include <stdio.h>

m6502Context context;
uint8_t mainMemory[64 * 1024];
void (*instruction_map[256]) (m6502Instruction*);

void initialize(void)
{
    memset(&context, 0, sizeof(m6502Context));
    memset(mainMemory, 0, 64 * 1024);

    context.memory = mainMemory;
    instruction_map[LDA] = &lda;
}

void print_context(void)
{
    printf("Context:\n");
    printf("\tPC: %d\n", context.pc);
    printf("\tAC: %d\n", context.ac);
    printf("\tX: %d\n", context.x);
    printf("\tY: %d\n", context.y);
}

void step(void)
{
    uint8_t opcode = mainMemory[context.pc++];
    m6502Instruction inst = parse_opcode(opcode);
    execute_instruction(&inst);
}

m6502Instruction parse_opcode(uint8_t opcode)
{
    m6502Instruction inst;
    switch (opcode)
    {
    case 0xA9:
        inst = (m6502Instruction){LDA, IMMEDIATE, 2, 2};
        break;
    case 0xAD:
        inst = (m6502Instruction){LDA, ABSOLUTE, 3, 2};
        break;
    default:
        break;
    }

    return inst;
}

void execute_instruction(m6502Instruction *instruction)
{
    instruction_map[instruction->instruction](instruction);
}

void lda(m6502Instruction *instruction)
{
    m6502Word operand;
    // Copy either 1 or 2 byte operand
    memcpy(&operand.w, &context.memory[context.pc], instruction->size-1);
    context.pc += instruction->size-1;

    switch (instruction->addrMode)
    {
        case IMPLIED:
            break;
        case IMMEDIATE:
            context.ac = operand.l;
            break;
        case ABSOLUTE:
            context.ac = context.memory[operand.w];
            break;
    }
}
