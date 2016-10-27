#ifndef TEST_MIPS_YMH15_H
#define TEST_MIPS_YMH15_H

#include "../../include/mips.h"
#include <cstdlib>
#include <ctime>

// Functions
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

uint32_t gen_instruction(uint32_t src1, uint32_t src2, uint32_t dest,
                         uint32_t shift, uint32_t function);
uint32_t gen_instruction(uint32_t opcode, uint32_t src, uint32_t dest,
                         uint32_t Astart);
uint32_t gen_instruction(uint32_t opcode, uint32_t memory);
uint32_t change_endianness(uint32_t inst);

int test_add(mips_mem_h ram, mips_cpu_h cpu, uint32_t type, uint32_t max,
             uint8_t value, unsigned i_t);
int test_bitwise(mips_mem_h ram, mips_cpu_h cpu, uint8_t op);
int test_I(mips_mem_h ram, mips_cpu_h cpu, uint32_t type, uint32_t num1,
              uint32_t num2);
int test_branch(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint32_t a, uint8_t l, uint32_t opc2, uint32_t b);
int test_jump(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode);
int test_load(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint32_t word);
int test_mult_div(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint32_t num1, uint32_t num2);

#endif // TEST_MIPS_YMH15_H
