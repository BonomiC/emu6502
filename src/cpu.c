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
    printf("\tC: %#x\n", context.sr.c);
    printf("\tV: %#x\n", context.sr.v);
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

    {&cmp, 0xC9, IMMEDIATE, 2, 2},
    {&cmp, 0xC5, ZERO_PAGE, 2, 3},
    {&cmp, 0xD5, ZERO_PAGE_X, 2, 4},
    {&cmp, 0xCD, ABSOLUTE, 3, 4},
    {&cmp, 0xDD, ABSOLUTE_X, 3, 4},
    {&cmp, 0xD9, ABSOLUTE_Y, 3, 4},
    {&cmp, 0xC1, INDIRECT_X, 2, 6},
    {&cmp, 0xD1, INDIRECT_Y, 2, 5},

    {&cpx, 0xE0, IMMEDIATE, 2, 2},
    {&cpx, 0xE4, ZERO_PAGE, 2, 3},
    {&cpx, 0xEC, ABSOLUTE, 3, 4},

    {&cpy, 0xC0, IMMEDIATE, 2, 2},
    {&cpy, 0xC4, ZERO_PAGE, 2, 3},
    {&cpy, 0xCC, ABSOLUTE, 3, 4},

    {&dec, 0xC6, ZERO_PAGE, 2, 5},
    {&dec, 0xD6, ZERO_PAGE_X, 2, 6},
    {&dec, 0xCE, ABSOLUTE, 3, 6},
    {&dec, 0xDE, ABSOLUTE_X, 3, 7},

    {&dex, 0xCA, IMPLIED, 1, 2},

    {&dey, 0x88, IMPLIED, 1, 2},

    {&eor, 0x49, IMMEDIATE, 2, 2},
    {&eor, 0x45, ZERO_PAGE, 2, 3},
    {&eor, 0x55, ZERO_PAGE_X, 2, 4},
    {&eor, 0x4D, ABSOLUTE, 3, 4},
    {&eor, 0x5D, ABSOLUTE_X, 3, 4},
    {&eor, 0x59, ABSOLUTE_Y, 3, 4},
    {&eor, 0x41, INDIRECT_X, 2, 6},
    {&eor, 0x51, INDIRECT_Y, 2, 5},

    {&inc, 0xE6, ZERO_PAGE, 2, 5},
    {&inc, 0xF6, ZERO_PAGE_X, 2, 6},
    {&inc, 0xEE, ABSOLUTE, 3, 6},
    {&inc, 0xFE, ABSOLUTE_X, 3, 7},

    {&inx, 0xE8, IMPLIED, 1, 2},

    {&iny, 0xC8, IMPLIED, 1, 2},

    {&jmp, 0x4C, ABSOLUTE, 3, 3},
    {&jmp, 0x6C, INDIRECT, 3, 5},

    {&jsr, 0x20, ABSOLUTE, 3, 6},

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

    {&lsr, 0x4A, IMPLIED, 1, 2},
    {&lsr, 0x46, ZERO_PAGE, 2, 5},
    {&lsr, 0x56, ZERO_PAGE_X, 2, 6},
    {&lsr, 0x4E, ABSOLUTE, 3, 6},
    {&lsr, 0x5E, ABSOLUTE_X, 3, 7},

    {&nop, 0xEA, IMPLIED, 1, 2},

    {&ora, 0x09, IMMEDIATE, 2, 2},
    {&ora, 0x05, ZERO_PAGE, 2, 3},
    {&ora, 0x15, ZERO_PAGE_X, 2, 4},
    {&ora, 0x0D, ABSOLUTE, 3, 4},
    {&ora, 0x1D, ABSOLUTE_X, 3, 4},
    {&ora, 0x19, ABSOLUTE_Y, 3, 4},
    {&ora, 0x01, INDIRECT_X, 2, 6},
    {&ora, 0x11, INDIRECT_Y, 2, 5},

    {&pha, 0x48, IMPLIED, 1, 3},

    {&php, 0x08, IMPLIED, 1, 3},

    {&pla, 0x68, IMPLIED, 1, 4},

    {&plp, 0x28, IMPLIED, 1, 4},

    {&rol, 0x2A, IMPLIED, 1, 2},
    {&rol, 0x26, ZERO_PAGE, 2, 5},
    {&rol, 0x36, ZERO_PAGE_X, 2, 6},
    {&rol, 0x2E, ABSOLUTE, 3, 6},
    {&rol, 0x3E, ABSOLUTE_X, 3, 7},

    {&ror, 0x6A, IMPLIED, 1, 2},
    {&ror, 0x66, ZERO_PAGE, 2, 5},
    {&ror, 0x76, ZERO_PAGE_X, 2, 6},
    {&ror, 0x6E, ABSOLUTE, 3, 6},
    {&ror, 0x7E, ABSOLUTE_X, 3, 7},

    {&rti, 0x40, IMPLIED, 1, 6},

    {&rts, 0x60, IMPLIED, 1, 6},

    {&sbc, 0xE9, IMMEDIATE, 2, 2},
    {&sbc, 0xE5, ZERO_PAGE, 2, 3},
    {&sbc, 0xF5, ZERO_PAGE_X, 2, 4},
    {&sbc, 0xED, ABSOLUTE, 3, 4},
    {&sbc, 0xFD, ABSOLUTE_X, 3, 4},
    {&sbc, 0xF9, ABSOLUTE_Y, 3, 4},
    {&sbc, 0xE1, INDIRECT_X, 2, 6},
    {&sbc, 0xF1, INDIRECT_Y, 2, 5},

    {&sec, 0x38, IMPLIED, 1, 2},

    {&sed, 0xF8, IMPLIED, 1, 2},

    {&sei, 0x78, IMPLIED, 1, 2},

    {&sta, 0x85, ZERO_PAGE, 2, 3},
    {&sta, 0x95, ZERO_PAGE_X, 2, 4},
    {&sta, 0x8D, ABSOLUTE, 3, 4},
    {&sta, 0x9D, ABSOLUTE_X, 3, 5},
    {&sta, 0x99, ABSOLUTE_Y, 3, 5},
    {&sta, 0x81, INDIRECT_X, 2, 6},
    {&sta, 0x91, INDIRECT_Y, 2, 6},

    {&stx, 0x86, ZERO_PAGE, 2, 3},
    {&stx, 0x96, ZERO_PAGE_Y, 2, 4},
    {&stx, 0x8E, ABSOLUTE, 3, 4},

    {&sty, 0x84, ZERO_PAGE, 2, 3},
    {&sty, 0x94, ZERO_PAGE_X, 2, 4},
    {&sty, 0x8C, ABSOLUTE, 3, 4},

    {&tax, 0xAA, IMPLIED, 1, 2},

    {&tay, 0xA8, IMPLIED, 1, 2},

    {&tsx, 0xBA, IMPLIED, 1, 2},

    {&txa, 0x8A, IMPLIED, 1, 2},

    {&txs, 0x9A, IMPLIED, 1, 2},

    {&tya, 0x98, IMPLIED, 1, 2}
};
