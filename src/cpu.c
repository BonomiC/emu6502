#include "cpu.h"
#include <stdio.h>

m6502Context context;
uint8_t mainMemory[MEMORY_SIZE];
m6502Instruction instruction_vector[MAX_INSTRUCTION_SIZE];

void initialize(void)
{
    memset(&context, 0, sizeof(m6502Context));
    memset(mainMemory, 0, MEMORY_SIZE);

    context.sp = 0xFF; // Common for stack pointer to init to end

    context.memory = mainMemory;

    build_instruction_vector();
}

void build_instruction_vector(void)
{
    for (uint16_t i = 0; i < MAX_INSTRUCTION_SIZE; i++)
    {
        m6502Instruction instr = instructions[i];
        if (instr.exec != NULL)
        {
            instruction_vector[instr.opcode] = instr;
        }
    }
}

void print_context(void)
{
    printf("Context:\n");
    printf("\tPC: 0x%04x\n", context.pc);
    printf("\tSP: %#x\n", context.sp);
    printf("\tA: %#x\n", context.a);
    printf("\tX: %#x\n", context.x);
    printf("\tY: %#x\n", context.y);
}

void push_stack(uint8_t value)
{
    mainMemory[STACK_START + context.sp] = value;
    context.sp--;
}

uint8_t pop_stack(void)
{
    context.sp++;
    return mainMemory[STACK_START + context.sp];
}

void push_stack_word(uint16_t value)
{
    m6502Word word;
    word.w = value;
    push_stack(word.h);
    push_stack(word.l);
}

uint16_t pop_stack_word(void)
{
    m6502Word word;
    word.l = pop_stack();
    word.h = pop_stack();
    return word.w;
}

void step(void)
{
    // Get the next opcode from memory
    uint8_t opcode = mainMemory[context.pc++];

    // Grab the correct instruction for the opcode
    m6502Instruction *instruction = &instruction_vector[opcode];

    if (instruction->exec == NULL)
    {
        printf("Instruction %#x has not yet been implemented!!! Skipping...\n", opcode);
        return;
    }

    // Extract any operand
    m6502Word operand;
    memcpy(&operand.w, &context.memory[context.pc], instruction->size-1);
    context.pc += instruction->size-1;
    instruction->operand = operand;

    // Get value based on address mode
    switch (instruction->addrMode)
    {
    case IMPLIED:
        break;
    case IMMEDIATE:
        instruction->value = operand.l;
        break;
    case ZERO_PAGE:
        instruction->address = operand.l;
        instruction->value = context.memory[instruction->address];
        break;
    case ZERO_PAGE_X:
        instruction->address = (uint16_t)operand.l + context.x;
        instruction->value = context.memory[instruction->address];
        break;
    case ZERO_PAGE_Y:
        instruction->address = (uint16_t)operand.l + context.y;
        instruction->value = context.memory[instruction->address];
        break;
    case ABSOLUTE:
        instruction->address = operand.w;
        instruction->value = context.memory[instruction->address];
        break;
    case ABSOLUTE_X:
        instruction->address = operand.w + context.x;
        instruction->value = context.memory[instruction->address];
        break;
    case ABSOLUTE_Y:
        instruction->address = operand.w + context.y;
        instruction->value = context.memory[instruction->address];
        break;
    case INDIRECT:
        memcpy(&instruction->address, context.memory + operand.w, sizeof(instruction->address));
        instruction->value = context.memory[instruction->address];
        break;
    case INDIRECT_X:
        instruction->address = context.memory[(uint16_t)operand.l + context.x];
        instruction->value = context.memory[instruction->address];
        break;
    case INDIRECT_Y:
    {
        instruction->address = context.memory[operand.l];
        instruction->address += context.y;
        instruction->value = context.memory[instruction->address];
        break;
    }
    case RELATIVE:
        instruction->address = context.pc + operand.w;
        instruction->value = operand.l;
        break;
    }

    // Execute the instruction
    instruction->exec(instruction);
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

    {&and, 0x29, IMMEDIATE, 2, 2},
    {&and, 0x25, ZERO_PAGE, 2, 3},
    {&and, 0x35, ZERO_PAGE_X, 2, 4},
    {&and, 0x2D, ABSOLUTE, 3, 4},
    {&and, 0x3D, ABSOLUTE_X, 3, 4},
    {&and, 0x39, ABSOLUTE_Y, 3, 4},
    {&and, 0x21, INDIRECT_X, 2, 6},
    {&and, 0x31, INDIRECT_Y, 2, 5},

    {&asl, 0x0A, IMPLIED, 1, 2},
    {&asl, 0x06, ZERO_PAGE, 2, 5},
    {&asl, 0x16, ZERO_PAGE_X, 2, 6},
    {&asl, 0x0E, ABSOLUTE, 3, 6},
    {&asl, 0x1E, ABSOLUTE_X, 3, 7},

    {&bcc, 0x90, RELATIVE, 2, 2},

    {&bcs, 0xB0, RELATIVE, 2, 2},

    {&beq, 0xF0, RELATIVE, 2, 2},

    {&bit, 0x24, ZERO_PAGE, 2, 3},
    {&bit, 0x2C, ABSOLUTE, 3, 4},

    {&bmi, 0x30, RELATIVE, 2, 2},

    {&bne, 0xD0, RELATIVE, 2, 2},

    {&bpl, 0x10, RELATIVE, 2, 2},

    {&brk, 0x00, IMMEDIATE, 2, 7},

    {&bvc, 0x50, RELATIVE, 2, 2},

    {&bvs, 0x70, RELATIVE, 2, 2},

    {&clc, 0x18, IMPLIED, 1, 2},

    {&cld, 0xD8, IMPLIED, 1, 2},

    {&cli, 0x58, IMPLIED, 1, 2},

    {&clv, 0xB8, IMPLIED, 1, 2},

    {&eor, 0x49, IMMEDIATE, 2, 2},
    {&eor, 0x45, ZERO_PAGE, 2, 3},
    {&eor, 0x55, ZERO_PAGE_X, 2, 4},
    {&eor, 0x4D, ABSOLUTE, 3, 4},
    {&eor, 0x5D, ABSOLUTE_X, 3, 4},
    {&eor, 0x59, ABSOLUTE_Y, 3, 4},
    {&eor, 0x41, INDIRECT_X, 2, 6},
    {&eor, 0x51, INDIRECT_Y, 2, 5},

    {&jmp, 0x4C, ABSOLUTE, 3, 3},
    {&jmp, 0x6C, INDIRECT, 3, 5},

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
    {&ldy, 0xBC, ABSOLUTE_X, 3, 4},

    {&pha, 0x48, IMPLIED, 1, 3},

    {&php, 0x08, IMPLIED, 1, 3},

    {&pla, 0x68, IMPLIED, 1, 4},

    {&plp, 0x28, IMPLIED, 1, 4},

    {&tsx, 0xBA, IMPLIED, 1, 2},

    {&txs, 0x9A, IMPLIED, 1, 2}
};
