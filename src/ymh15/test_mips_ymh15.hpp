#ifndef TEST_MIPS_YMH15_H
#define TEST_MIPS_YMH15_H

#include "../../include/mips.h"
#include <cstdlib>
#include <ctime>

// Functions
#define JR 0x8
#define JALR 0x9
#define ADD 0x20
#define ADDU 0x21
#define SUB 0x22
#define SUBU 0x23
#define AND 0x24
#define OR 0x25
#define XOR 0x26

// OPcodes
#define BGEZ 0x1
#define J 0x2
#define JAL 0x3
#define BEQ 0x4
#define BNE 0x5
#define BLEZ 0x6
#define BGTZ 0x7
#define ADDI 0x8
#define ADDIU 0x9
#define ANDI 0xc
#define ORI 0xd
#define XORI 0xe
#define LB 0x20
#define LBU 0x24

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
int test_load(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint8_t num);

#endif // TEST_MIPS_YMH15_H
