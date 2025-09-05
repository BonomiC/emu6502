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
