#ifndef TEST_MIPS_YMH15_H
#define TEST_MIPS_YMH15_H

#include "../../include/mips.h"

uint32_t gen_instruction(uint32_t src1, uint32_t src2, uint32_t dest,
                         uint32_t shift, uint32_t function);
uint32_t gen_instruction(uint32_t opcode, uint32_t src, uint32_t dest,
                         uint32_t Astart);
uint32_t gen_instruction(uint32_t opcode, uint32_t memory);
uint32_t change_endianness(uint32_t inst);

void test_endian(mips_mem_h ram);

#endif // TEST_MIPS_YMH15_H
