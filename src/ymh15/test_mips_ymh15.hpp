#ifndef TEST_MIPS_YMH15_H
#define TEST_MIPS_YMH15_H

#include "../../include/mips.h"
#include <cstdlib>
#include <ctime>

uint32_t gen_instruction(uint32_t src1, uint32_t src2, uint32_t dest,
                         uint32_t shift, uint32_t function);
uint32_t gen_instruction(uint32_t opcode, uint32_t src, uint32_t dest,
                         uint32_t Astart);
uint32_t gen_instruction(uint32_t opcode, uint32_t memory);
uint32_t change_endianness(uint32_t inst);

int test_add(mips_mem_h ram, mips_cpu_h cpu, uint32_t type, uint32_t max, uint8_t value, unsigned i_t);

#endif // TEST_MIPS_YMH15_H
