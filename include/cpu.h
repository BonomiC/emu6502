#ifndef CPU_H_
#define CPU_H_

#include "types.h"
#include "isa.h"
#include <stdint.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))>>(pos)

#define MEMORY_SIZE (64 * 1024)

// NMI (Non-Maskable Interrupt) vector, 16-bit (LB, HB)
#define NMI_L 0xFFFA
#define NMI_H 0xFFFB

// RES (Reset) vector, 16-bit (LB, HB)
#define RES_L 0xFFFC
#define RES_H 0xFFFD

// IRQ (Interrupt Request) vector, 16-bit (LB, HB)
#define IRQ_L 0xFFFE
#define IRQ_H 0xFFFF

extern m6502Context context;
extern uint8_t mainMemory[MEMORY_SIZE]; // 64K Memory

extern m6502Instruction instruction_vector[MAX_INSTRUCTION_SIZE];

void initialize(void);
void build_instruction_vector(void);
void print_context(void);

void step(void);
m6502Instruction parse_opcode(uint8_t opcode);
void execute_instruction(m6502Instruction *instruction);

// These functions are defined in `instructions.c` to keep `cpu.c` cleaner
void adc(m6502Instruction *instruction);
void and(m6502Instruction *instruction);
void asl(m6502Instruction *instruction);
void bcc(m6502Instruction *instruction);
void lda(m6502Instruction *instruction);
void ldx(m6502Instruction *instruction);
void ldy(m6502Instruction *instruction);

#endif /* CPU_H_ */
