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
    if (instruction->addrMode == ACCUMULATOR)
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
