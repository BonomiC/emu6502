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
#define IRQ_HANDLER 0xFFFE
#define IRQ_L 0xFFFE
#define IRQ_H 0xFFFF

#define STACK_START 0x0100

extern m6502Context context;
extern uint8_t mainMemory[MEMORY_SIZE]; // 64K Memory

extern m6502Instruction instruction_vector[MAX_INSTRUCTION_SIZE];

void initialize(void);
void build_instruction_vector(void);
void print_context(void);

void push_stack(uint8_t value);
uint8_t pop_stack(void);
void push_stack_word(uint16_t value);
uint16_t pop_stack_word(void);

void step(void);
m6502Instruction parse_opcode(uint8_t opcode);
void execute_instruction(m6502Instruction *instruction);

// These functions are defined in `instructions.c` to keep `cpu.c` cleaner
void adc(m6502Instruction *instruction);
void and(m6502Instruction *instruction);
void asl(m6502Instruction *instruction);
void bcc(m6502Instruction *instruction);
void bcs(m6502Instruction *instruction);
void beq(m6502Instruction *instruction);
void bit(m6502Instruction *instruction);
void bmi(m6502Instruction *instruction);
void bne(m6502Instruction *instruction);
void bpl(m6502Instruction *instruction);
void brk(m6502Instruction *instruction);
void bvc(m6502Instruction *instruciton);
void bvs(m6502Instruction *instruction);
void clc(m6502Instruction *instruction);
void cld(m6502Instruction *instruction);
void cli(m6502Instruction *instruction);
void clv(m6502Instruction *instruction);
void cmp(m6502Instruction *instruction);
void cpx(m6502Instruction *instruction);
void cpy(m6502Instruction *instruction);
void dec(m6502Instruction *instruction);
void dex(m6502Instruction *instruction);
void dey(m6502Instruction *instruction);
void inc(m6502Instruction *instruction);
void inx(m6502Instruction *instruction);
void iny(m6502Instruction *instruction);
void eor(m6502Instruction *instruction);
void jmp(m6502Instruction *instruction);
void jsr(m6502Instruction *instruction);
void lda(m6502Instruction *instruction);
void ldx(m6502Instruction *instruction);
void ldy(m6502Instruction *instruction);
void lsr(m6502Instruction *instruction);
void nop(m6502Instruction *instruction);
void ora(m6502Instruction *instruction);
void pha(m6502Instruction *instruction);
void php(m6502Instruction *instruction);
void pla(m6502Instruction *instruction);
void plp(m6502Instruction *instruction);
void rol(m6502Instruction *instruction);
void ror(m6502Instruction *instruction);
void rti(m6502Instruction *instruction);
void rts(m6502Instruction *instruction);
void sec(m6502Instruction *instruction);
void sed(m6502Instruction *instruction);
void sei(m6502Instruction *instruction);
void sta(m6502Instruction *instruction);
void stx(m6502Instruction *instruction);
void sty(m6502Instruction *instruction);
void tax(m6502Instruction *instruction);
void tay(m6502Instruction *instruction);
void tsx(m6502Instruction *instruction);
void txa(m6502Instruction *instruction);
void txs(m6502Instruction *instruction);
void tya(m6502Instruction *instruction);

#endif /* CPU_H_ */
