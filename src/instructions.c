#include "cpu.h"

void adc(m6502Instruction *instruction)
{
    m6502Word res = {.w=(uint16_t)context.a + instruction->value + context.sr.c};
    context.sr.c = res.w > 0xFF;
    context.sr.z = res.w == 0;
    context.sr.v = CHECK_BIT((res.w ^ context.a) & (res.w ^ instruction->value), 7);
    context.sr.n = CHECK_BIT(res.w, 7);
    context.a = res.l;
}

void and(m6502Instruction *instruction)
{
    context.a = context.a & instruction->value;
    context.sr.z = context.a == 0;
    context.sr.n = CHECK_BIT(context.a, 7);
}

void asl(m6502Instruction *instruction)
{
    if (instruction->addrMode == IMPLIED)
    {
        context.sr.c = CHECK_BIT(context.a, 7);
        context.a <<= 1;
        context.sr.z = context.a == 0;
        context.sr.n = CHECK_BIT(context.a, 7);
        return;
    }

    context.sr.c = CHECK_BIT(instruction->value, 7);
    instruction->value <<= 1;
    context.sr.z = instruction->value == 0;
    context.sr.n = CHECK_BIT(instruction->value, 7);

    // Write the new value back to memory
    context.memory[instruction->address] = instruction->value;
}

void bcc(m6502Instruction *instruction)
{
    if (!context.sr.c)
    {
        int8_t offset = (int8_t)instruction->operand.l;
        context.pc += offset;
    }
}

void bcs(m6502Instruction *instruction)
{
    if (context.sr.c)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void beq(m6502Instruction *instruction)
{
    if (context.sr.z)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void bit(m6502Instruction *instruction)
{
    uint8_t result = context.a & instruction->value;
    context.sr.z = result == 0;
    context.sr.v = CHECK_BIT(result, 6);
    context.sr.n = CHECK_BIT(result, 7);
}

void bmi(m6502Instruction *instruction)
{
    if (context.sr.n)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void bne(m6502Instruction *instruction)
{
    if (!context.sr.z)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void bpl(m6502Instruction *instruction)
{
    if (!context.sr.n)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void brk(m6502Instruction *instruction)
{
    push_stack_word(context.pc);
    m6502StatusReg sr = context.sr;
    sr.x = 1;
    sr.b = 1;
    sr.i = 1;
    push_stack(*((uint8_t*)&sr)); // Seems like a hack, definitely test this!
    memcpy(&context.pc, mainMemory + IRQ_HANDLER, 2);
}

void bvc(m6502Instruction *instruction)
{
    if (!context.sr.v)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void bvs(m6502Instruction *instruction)
{
    if (context.sr.v)
    {
        context.pc += (int8_t)instruction->operand.l;
    }
}

void clc(m6502Instruction *instruction)
{
    context.sr.c = 0;
}

void cld(m6502Instruction *instruction)
{
    context.sr.d = 0;
}

void cli(m6502Instruction *instruction)
{
    context.sr.i = 0;
}

void clv(m6502Instruction *instruction)
{
    context.sr.v = 0;
}

void cmp(m6502Instruction *instruction)
{
    uint8_t result = context.a - instruction->value;
    context.sr.z = result = 0;
    context.sr.c = context.a >= instruction->value;
    context.sr.n = CHECK_BIT(result, 7);
}

void cpx(m6502Instruction *instruction)
{
    uint8_t result = context.x - instruction->value;
    context.sr.z = result = 0;
    context.sr.c = context.x >= instruction->value;
    context.sr.n = CHECK_BIT(result, 7);
}

void cpy(m6502Instruction *instruction)
{
    uint8_t result = context.y - instruction->value;
    context.sr.z = result = 0;
    context.sr.c = context.y >= instruction->value;
    context.sr.n = CHECK_BIT(result, 7);
}

void dec(m6502Instruction *instruction)
{
    mainMemory[instruction->address] -= 1;
    context.sr.z = mainMemory[instruction->address] == 0;
    context.sr.n = CHECK_BIT(mainMemory[instruction->address], 7);
}

void dex(m6502Instruction *instruction)
{
    context.x--;
    context.sr.z = context.x == 0;
    context.sr.n = CHECK_BIT(context.x, 7);
}

void dey(m6502Instruction *instruction)
{
    context.y--;
    context.sr.z = context.y == 0;
    context.sr.n = CHECK_BIT(context.y, 7);
}

void eor(m6502Instruction *instruction)
{
    context.a = context.a | instruction->value;
    context.sr.z = context.a == 0;
    context.sr.n = CHECK_BIT(context.a, 7);
}

void inc(m6502Instruction *instruction)
{
    mainMemory[instruction->address] += 1;
    context.sr.z = mainMemory[instruction->address] == 0;
    context.sr.n = CHECK_BIT(mainMemory[instruction->address], 7);
}

void inx(m6502Instruction *instruction)
{
    context.x++;
    context.sr.z = context.x == 0;
    context.sr.n = CHECK_BIT(context.x, 7);
}

void iny(m6502Instruction *instruction)
{
    context.y++;
    context.sr.z = context.y == 0;
    context.sr.n = CHECK_BIT(context.y, 7);
}

void jmp(m6502Instruction *instruction)
{
    context.pc = instruction->operand.w;
}

void lda(m6502Instruction *instruction)
{
    uint8_t value = instruction->value;
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT(value, 7);
    context.a = value;
}

void ldx(m6502Instruction *instruction)
{
    uint8_t value = instruction->value;
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT(value, 7);
    context.x = value;
}

void ldy(m6502Instruction *instruction)
{
    uint8_t value = instruction->value;
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT(value, 7);
    context.y = value;
}

void pha(m6502Instruction *instruction)
{
    push_stack(context.a);
}

void php(m6502Instruction *instruction)
{
    m6502StatusReg sr = context.sr;
    sr.x = 1;
    sr.b = 1;
    push_stack(*((uint8_t*)&sr)); // Seems like a hack, definitely test this!
}

void pla(m6502Instruction *instruction)
{
    context.a = pop_stack();
}

void plp(m6502Instruction *instruction)
{
    uint8_t val = pop_stack();
    m6502StatusReg sr = *(m6502StatusReg*)&val; // Seems like a hack, definitely test this!
    uint8_t mask = 0xCF;
    uint8_t result = (*(uint8_t*)&context.sr & ~mask) | (*(uint8_t*)&sr & mask);
    context.sr = *(m6502StatusReg*)&result;
}

void tsx(m6502Instruction *instruction)
{
    context.x = context.sp;
    context.sr.z = context.x == 0;
    context.sr.n = CHECK_BIT(context.x, 7);
}

void txs(m6502Instruction *instruction)
{
    context.sp = context.x;
}
