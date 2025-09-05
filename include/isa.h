#ifndef ISA_H_
#define ISA_H_

#include "types.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTION_SIZE 256

typedef enum
{
    IMPLIED,
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT_X,
    INDIRECT_Y,
    RELATIVE
} addrMode_e;

typedef struct m6502Instruction
{
    void (*exec)(struct m6502Instruction*, m6502Word);
    uint8_t opcode;
    addrMode_e addrMode;
    uint8_t size;
    uint8_t cycles;
} m6502Instruction;

extern m6502Instruction instructions[MAX_INSTRUCTION_SIZE];

#endif /* ISA_H_ */
