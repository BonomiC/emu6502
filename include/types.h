#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef union m6502Word
{
    uint16_t w;
    struct
    {
        uint8_t l, h;
    };
} m6502Word;

typedef struct m6502StatusReg
{
    uint8_t c : 1; // Carry
    uint8_t z : 1; // Zero
    uint8_t i : 1; // Interrupt
    uint8_t d : 1; // Decimal
    uint8_t b : 1; // Break
    uint8_t x : 1; // ignored
    uint8_t v : 1; // Overflow
    uint8_t n : 1; // Negative
} m6502StatusReg;

typedef uint8_t m6502Reg;

typedef struct m6502Context
{
    // PC is 16bit, but is often accessed as high/low bytes individually
    union
    {
        uint16_t pc;
        struct
        {
            uint8_t pcl;
            uint8_t pch;
        };
    };

    m6502Reg a;
    m6502Reg x;
    m6502Reg y;
    m6502Reg sp;
    m6502StatusReg sr;

    uint8_t halted;
    uint8_t intEnabled;
    uint8_t intPending;
    uint16_t NMIAddress;
    uint8_t NMIPending;

    uint8_t *memory;
} m6502Context;

#endif /* TYPES_H_ */
