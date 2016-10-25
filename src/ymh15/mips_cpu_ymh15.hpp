#ifndef MIPS_CPU_YMH15_H
#define MIPS_CPU_YMH15_H

#include <cstdio>

#include "../../include/mips.h"

// defines the location of the things in the array var
#define OPCODE 0
#define REG_S 1
#define REG_T 2
#define REG_D 3
#define SHIFT 4
#define FUNC 5
#define IMM 6
#define MEM 7

// executes an instruction by first decoding it and sending the right
// variables to the different functions to be correctly interpreted
mips_error exec_instruction(mips_cpu_h state, uint32_t inst);
mips_error exec_R(mips_cpu_h state, uint32_t var[8]);
mips_error exec_J(mips_cpu_h state, uint32_t var[8]);
mips_error exec_I(mips_cpu_h state, uint32_t var[8]);

// performs addition and subtraction
mips_error add_sub(mips_cpu_h state, uint32_t var[8], int32_t add_sub,
                   int32_t imm);
// performs bitwise operations
mips_error bitwise(mips_cpu_h state, uint32_t var[8], unsigned imm);

mips_error branch(mips_cpu_h state, uint32_t var[8]);

mips_error jump(mips_cpu_h state, uint32_t var[8], uint8_t link);

mips_error load(mips_cpu_h state, uint32_t var[8]);
mips_error store(mips_cpu_h state, uint32_t var[8]);

#endif // MIPS_CPU_YMH15_H
