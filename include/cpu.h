#ifndef M6502_H_
#define M6502_H_

#include "isa.h"
#include <stdint.h>

// NMI (Non-Maskable Interrupt) vector, 16-bit (LB, HB)
#define NMI_L 0xFFFA
#define NMI_H 0xFFFB

// RES (Reset) vector, 16-bit (LB, HB)
#define RES_L 0xFFFC
#define RES_H 0xFFFD

// IRQ (Interrupt Request) vector, 16-bit (LB, HB)
#define IRQ_L 0xFFFE
#define IRQ_H 0xFFFF

typedef union
{
    uint16_t w;
    struct
    {
        uint8_t l, h;
    };
} m6502Word;

typedef struct
{
    uint8_t n : 1; // Negative
    uint8_t v : 1; // Overflow
    uint8_t   : 1; // ignored
    uint8_t b : 1; // Break
    uint8_t d : 1; // Decimal
    uint8_t i : 1; // Interrupt
    uint8_t z : 1; // Zero
    uint8_t c : 1; // Carry
} m6502StatusReg;

typedef uint8_t m6502Reg;

typedef struct
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

    m6502Reg ac;
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

extern m6502Context context;
extern uint8_t mainMemory[64 * 1024]; // 64K Memory

extern void (*instruction_map[256]) (m6502Instruction*);

void initialize(void);
void print_context(void);

void step(void);
m6502Instruction parse_opcode(uint8_t opcode);
void execute_instruction(m6502Instruction *instruction);

void lda(m6502Instruction *instruction);

#endif /* M6502_H_ */
