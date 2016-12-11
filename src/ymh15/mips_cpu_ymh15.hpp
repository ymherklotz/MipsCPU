#ifndef MIPS_CPU_YMH15_H
#define MIPS_CPU_YMH15_H

#include <cstdio>

#include "mips.h"

// defines the location of the things in the array var
#define OPCODE 0
#define REG_S 1
#define REG_T 2
#define REG_D 3
#define SHIFT 4
#define FUNC 5
#define IMM 6
#define MEM 7

// Functions
#define NOOP 0x0
#define SLL 0x0
#define SRL 0x2
#define SRA 0x3
#define SLLV 0x4
#define SRLV 0x6
#define SRAV 0x7
#define JR 0x8
#define JALR 0x9
#define MFHI 0x10
#define MTHI 0x11
#define MFLO 0x12
#define MTLO 0x13
#define MULT 0x18
#define MULTU 0x19
#define DIV 0x1a
#define DIVU 0x1b
#define ADD 0x20
#define ADDU 0x21
#define SUB 0x22
#define SUBU 0x23
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define SLT 0x2a
#define SLTU 0x2b

// OPcodes
#define BGEZ 0x1
#define BGEZAL 0x1
#define BLTZ 0x1
#define BLTZAL 0x1
#define J 0x2
#define JAL 0x3
#define BEQ 0x4
#define BNE 0x5
#define BLEZ 0x6
#define BGTZ 0x7
#define ADDI 0x8
#define ADDIU 0x9
#define SLTI 0xa
#define SLTIU 0xb
#define ANDI 0xc
#define ORI 0xd
#define XORI 0xe
#define LUI 0xf
#define LB 0x20
#define LH 0x21
#define LWL 0x22
#define LW 0x23
#define LBU 0x24
#define LHU 0x25
#define LWR 0x26
#define SB 0x28
#define SH 0x29
#define SW 0x2b

// executes an instruction by first decoding it and sending the right
// variables to the different functions to be correctly interpreted
mips_error exec_instruction(mips_cpu_h state, uint32_t inst);
mips_error exec_R(mips_cpu_h state, uint32_t var[8]);
mips_error exec_J(mips_cpu_h state, uint32_t var[8]);
mips_error exec_I(mips_cpu_h state, uint32_t var[8]);
// performs addition and subtraction
mips_error add_sub(mips_cpu_h state, uint32_t var[8], int32_t add_sub,int32_t imm);
// performs bitwise operations
mips_error bitwise(mips_cpu_h state, uint32_t var[8], unsigned imm);
mips_error branch(mips_cpu_h state, uint32_t var[8]);
mips_error jump(mips_cpu_h state, uint32_t var[8], uint8_t link);
mips_error load(mips_cpu_h state, uint32_t var[8]);
mips_error store(mips_cpu_h state, uint32_t var[8]);
mips_error move(mips_cpu_h state, uint32_t var[8]);
mips_error mult_div(mips_cpu_h state, uint32_t var[8]);
mips_error shift(mips_cpu_h state, uint32_t var[8]);
mips_error set(mips_cpu_h state, uint32_t var[8], uint32_t imm);

#endif // MIPS_CPU_YMH15_H
