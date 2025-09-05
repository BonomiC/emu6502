#include "cpu.h"

void adc(m6502Instruction *instruction, uint16_t value)
{
    m6502Word res = {.w=context.a + value + context.sr.c};
    context.sr.c = res.w > 0xFF;
    context.sr.z = res.w == 0;
    context.sr.v = CHECK_BIT((res.w ^ context.a) & (res.w ^ value), 7);
    context.sr.n = CHECK_BIT(res.w, 7);
    context.a = res.l;
}

void lda(m6502Instruction *instruction, uint16_t value)
{
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT((uint8_t)value, 7);
    context.a = (uint8_t)value;
}

void ldx(m6502Instruction *instruction, uint16_t value)
{
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT((uint8_t)value, 7);
    context.x = (uint8_t)value;
}

void ldy(m6502Instruction *instruction, uint16_t value)
{
    context.sr.z = value == 0;
    context.sr.n = CHECK_BIT((uint8_t)value, 7);
    context.y = (uint8_t)value;
}
