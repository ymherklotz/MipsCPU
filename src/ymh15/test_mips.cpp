#include "../../include/mips.h"
#include "test_mips_ymh15.hpp"

#include <iostream>

using namespace std;

int main() {
    mips_mem_h ram = mips_mem_create_ram(4096);
    mips_cpu_h cpu = mips_cpu_create(ram);

    srand(time(NULL));

    mips_test_begin_suite();

    int testId = mips_test_begin_test("ADD");
    mips_test_end_test(testId, test_add(ram, cpu, 0x20, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("ADD");
    mips_test_end_test(testId, !test_add(ram, cpu, 0x20, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("ADDU");
    mips_test_end_test(testId, test_add(ram, cpu, 0x21, 0x3fffffff, 0, 10), "testing without overflow");

    testId = mips_test_begin_test("ADDU");
    mips_test_end_test(testId, test_add(ram, cpu, 0x21, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("SUB");
    mips_test_end_test(testId, test_add(ram, cpu, 0x22, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("SUB");
    mips_test_end_test(testId, !test_add(ram, cpu, 0x22, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("SUBU");
    mips_test_end_test(testId, test_add(ram, cpu, 0x23, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("SUBU");
    mips_test_end_test(testId, test_add(ram, cpu, 0x23, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    
    mips_test_end_suite();
    return 0;
}

// R Type
uint32_t gen_instruction(uint32_t src1, uint32_t src2, uint32_t dest,
                     uint32_t shift, uint32_t function) {
    uint32_t inst = 0;
    inst = inst | src1 << 21 | src2 << 16 | dest << 11 | shift << 6 |
        function;
    return inst;
}

// I Type
uint32_t gen_instruction(uint32_t opcode, uint32_t src, uint32_t dest,
                     uint32_t Astart) {
    uint32_t inst = 0;
    inst = inst | opcode << 26 | src << 21 | dest << 16 | Astart;
    return inst;
}

// J Type
uint32_t gen_instruction(uint32_t opcode, uint32_t memory) {
    uint32_t inst = 0;
    inst = inst | opcode << 26 | memory;
    return inst;
}

uint32_t change_endianness(uint32_t inst) {
    inst = (inst << 24 | ((inst << 8)&0xff0000) |
            ((inst >> 8)&0xff00) |inst >> 24);
    return inst;
}

int test_add(mips_mem_h ram, mips_cpu_h cpu, uint32_t type, uint32_t max, uint8_t value, unsigned i_t) {
    
    uint32_t inst, ans, a, b;

    for(unsigned i = 0; i < i_t; ++i) {
        mips_error mips_err;
        mips_cpu_reset(cpu);
        inst = gen_instruction(9, 10, 8, 0, type);
        if(value) {
            a = max;
            if(type > 0x21) {
                b = -max;
            } else {
                b = max;
            }
        } else {
            a = rand() % max;
            b = rand() % max;
        }
        
        mips_mem_write(ram, 0, 4, (uint8_t*)&inst);
        
        mips_cpu_set_register(cpu, 9, a);
        mips_cpu_set_register(cpu, 10, b);
    
        mips_err = mips_cpu_step(cpu);
    
        mips_cpu_get_register(cpu, 8, &ans);

        if(type < 0x22) {
            printf("%#10x + %#10x = %#10x\n", a, b, ans);
            if(mips_err == mips_ExceptionArithmeticOverflow) {
                return 0;
            }
        } else {
            printf("%#10x - %#10x = %#10x\n", a, b, ans);
            if(mips_err == mips_ExceptionArithmeticOverflow) {
                return 0;
            }
        }

    }

    return 1;
}

