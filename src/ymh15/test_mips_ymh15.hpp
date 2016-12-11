#ifndef TEST_MIPS_YMH15_H
#define TEST_MIPS_YMH15_H

#include "mips.h"
#include <cstdlib>
#include <ctime>

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

uint32_t gen_instruction(uint32_t src1, uint32_t src2, uint32_t dest, uint32_t shift, uint32_t function);
uint32_t gen_instruction(uint32_t opcode, uint32_t src, uint32_t dest, uint32_t Astart);
uint32_t gen_instruction(uint32_t opcode, uint32_t memory);

void change_endianness(uint32_t &inst);

int check_reg(mips_cpu_h state, uint8_t reg_addr, uint32_t check_value);
int check_error(mips_error err, mips_error expected_err);
int check_mem(mips_mem_h mem, uint32_t addr, uint32_t length, uint32_t check_value);

mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t src1, uint32_t src2, uint32_t dest, uint32_t shift, uint32_t function);
mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t opcode, uint32_t src, uint32_t dest, uint32_t Astart);
mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t opcode, uint32_t memory);

mips_error rand_reg(mips_cpu_h state);

#endif
