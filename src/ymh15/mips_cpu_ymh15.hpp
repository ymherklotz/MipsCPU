#ifndef MIPS_CPU_YMH15_H
#define MIPS_CPU_YMH15_H

#include "../../include/mips.h"

#define OPCODE 0
#define REG_S 1
#define REG_T 2
#define REG_D 3
#define SHIFT 4
#define FUNC 5
#define IMM 6
#define MEM 7

mips_error read_instruction(mips_cpu_h state);
mips_error exec_instruction(mips_cpu_h state, uint32_t inst);
void exec_R(mips_cpu_h state, uint32_t var[8]);
void exec_J(mips_cpu_h state, uint32_t var[8]);
void exec_I(mips_cpu_h state, uint32_t var[8]);
void add_sub(mips_cpu_h state, uint32_t var[8]);

#endif // MIPS_CPU_YMH15_H
