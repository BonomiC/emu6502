#ifndef ISA_H_
#define ISA_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    LDA,
    LDX,
    LDY
} instruction_e;

typedef enum
{
    IMPLIED,
    IMMEDIATE,
    ABSOLUTE,
    ZERO_PAGE,
    INDEXED_ABSOLUTE,
    INDEXED_ZERO_PAGE,
    INDEXED_INDIRECT,
    POST_INDEXED_INDIRECT,
    RELATIVE
} addrMode_e;

typedef struct
{
    instruction_e instruction;
    addrMode_e addrMode;
    uint8_t size;
    uint8_t cycles;
} m6502Instruction;

extern m6502Instruction instructions[256];

#endif /* ISA_H_ */
