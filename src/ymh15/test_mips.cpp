#include "../../include/mips.h"
#include "test_mips_ymh15.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    mips_mem_h ram = mips_mem_create_ram(4096);
    mips_cpu_h cpu = mips_cpu_create(ram);

    srand(time(NULL));

    if(argc == 2) {
        mips_cpu_set_debug_level(cpu, argv[1][0]-'0', stdout);
    } else {
        mips_cpu_set_debug_level(cpu, 0, NULL);
    }

    mips_test_begin_suite();
    
    int testId = mips_test_begin_test("ADD");
    mips_test_end_test(testId, test_add(ram, cpu, ADD, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("ADD");
    mips_test_end_test(testId, !test_add(ram, cpu, ADD, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("ADDU");
    mips_test_end_test(testId, test_add(ram, cpu, ADDU, 0x3fffffff, 0, 10), "testing without overflow");

    testId = mips_test_begin_test("ADDU");
    mips_test_end_test(testId, test_add(ram, cpu, ADDU, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("SUB");
    mips_test_end_test(testId, test_add(ram, cpu, SUB, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("SUB");
    mips_test_end_test(testId, !test_add(ram, cpu, SUB, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("SUBU");
    mips_test_end_test(testId, test_add(ram, cpu, SUBU, 0x3fffffff, 0, 10), "Testing the adder without overflow");

    testId = mips_test_begin_test("SUBU");
    mips_test_end_test(testId, test_add(ram, cpu, SUBU, 0x7fffffff, 1, 1), "Testing the adder with overflow");

    testId = mips_test_begin_test("AND");
    mips_test_end_test(testId, test_bitwise(ram, cpu, AND), "Testing bitwise and");

    testId = mips_test_begin_test("OR");
    mips_test_end_test(testId, test_bitwise(ram, cpu, OR), "Testing bitwise or");

    testId = mips_test_begin_test("XOR");
    mips_test_end_test(testId, test_bitwise(ram, cpu, XOR), "Testing bitwise xor");

    testId = mips_test_begin_test("ADDI");
    mips_test_end_test(testId, test_I(ram, cpu, ADDI, 20, 10), "Testing addi");
    
    testId = mips_test_begin_test("ADDIU");
    mips_test_end_test(testId, test_I(ram, cpu, ADDIU, 20, 10), "Testing addiu");
    
    testId = mips_test_begin_test("ANDI");
    mips_test_end_test(testId, test_I(ram, cpu, ANDI, 20, 10), "Testing addi");
    
    testId = mips_test_begin_test("ORI");
    mips_test_end_test(testId, test_I(ram, cpu, ORI, 20, 10), "Testing addi");
    
    testId = mips_test_begin_test("XORI");
    mips_test_end_test(testId, test_I(ram, cpu, XORI, 20, 10), "Testing addi");

    testId = mips_test_begin_test("BEQ");
    mips_test_end_test(testId, test_branch(ram, cpu, 4, 4, 0, 9, 4), "Testing BEQ");

    testId = mips_test_begin_test("BNE");
    mips_test_end_test(testId, test_branch(ram, cpu, 5, 4, 0, 9, 10), "Testing BNE");

    testId = mips_test_begin_test("BGEZ");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, 4, 0, 1, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("BLEZ");
    mips_test_end_test(testId, test_branch(ram, cpu, 6, -1, 0, 0, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("BGTZ");
    mips_test_end_test(testId, test_branch(ram, cpu, 7, 4, 0, 0, 20), "Testing BGEZ");

    testId = mips_test_begin_test("BGEZAL");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, 4, 1, 17, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("BLTZAL");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 1, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("BLTZ");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 0, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("J");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 0, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("JAL");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 0, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("JALR");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 0, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("JR");
    mips_test_end_test(testId, test_branch(ram, cpu, 1, -1, 0, 16, 20), "Testing BGEZ");
    
    testId = mips_test_begin_test("LB");
    mips_test_end_test(testId, test_load(ram, cpu, LB, 0xff), "Testing LB");
    
    testId = mips_test_begin_test("LBU");
    mips_test_end_test(testId, test_load(ram, cpu, LBU, 0xff), "Testing LB");
    
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
        inst = change_endianness(gen_instruction(9, 10, 8, 0, type));
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
            //printf("%#10x + %#10x = %#10x\t%#10x\n", a, b, ans, mips_err);
            if(mips_err == mips_ExceptionArithmeticOverflow || a+b!=ans) {
                return 0;
            }
        } else {
            //printf("%#10x - %#10x = %#10x\t%#10x\n", a, b, ans, mips_err);
            if(mips_err == mips_ExceptionArithmeticOverflow || a-b!=ans) {
                return 0;
            }
        }
    }

    return 1;
}

int test_bitwise(mips_mem_h ram, mips_cpu_h cpu, uint8_t op) {
    uint32_t a, b, ans, inst;
    int passed;
    for(unsigned i = 0; i < 10; ++i) {
        passed = 0;
        mips_cpu_reset(cpu);
        inst = change_endianness(gen_instruction(8, 9, 7, 0, op));

        a = rand() % 0xffffffff;
        b = rand() % 0xffffffff;

        mips_mem_write(ram, 0, 4, (uint8_t*)&inst);

        mips_cpu_set_register(cpu, 8, a);
        mips_cpu_set_register(cpu, 9, b);

        mips_error mips_err = mips_cpu_step(cpu);

        mips_cpu_get_register(cpu, 7, &ans);

        if(op == AND) {
            //printf("%#10x & %#10x = %#10x\t%#10x\n", a, b, ans, mips_err);
            if((a & b) == ans) {
                passed = 1;
            }
        } else if(op == OR) {
            //printf("%#10x | %#10x = %#10x\t%#10x\n", a, b, ans, mips_err);
            if((a | b) == ans) {
                passed = 1;
            }
        } else {
            //printf("%#10x ^ %#10x = %#10x\t%#10x\n", a, b, ans, mips_err);
            if((a ^ b) == ans) {
                passed = 1;
            }
        }

        
        if(mips_err != mips_Success || !passed) {
            return 0;
        }
    }
    return 1;
}

int test_I(mips_mem_h ram, mips_cpu_h cpu, uint32_t type,
             uint32_t num1, uint32_t num2) {
    int passed = 0;
    uint32_t result, inst;

    mips_cpu_reset(cpu);
    inst = change_endianness(gen_instruction(type, 8, 7, num1));

    mips_mem_write(ram, 0, 4, (uint8_t*)&inst);
    mips_cpu_set_register(cpu, 8, num2);

    mips_cpu_step(cpu);

    mips_cpu_get_register(cpu, 7, &result);

    switch(type) {
    case 8:
        if(result == num1 + num2) {
            passed = 1;
        }
    case 9:
        if(result == num1 + num2) {
            passed = 1;
        }
    case 12:
        if(result == (num1 & num2)) {
            passed = 1;
        }
    case 13:
        if(result == (num1 | num2)) {
            passed = 1;
        }
    case 14:
        if(result == (num1 ^ num2)) {
            passed = 1;
        }
    default:;
    }

    return passed;
}

int test_branch(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint32_t a, uint8_t l, uint32_t opc2, uint32_t b) {
    int passed = 0;
    int pass = 0;
    uint32_t reg10, reg11, reg31, pc_set;
    pc_set = 32;
    mips_cpu_reset(cpu);
    mips_cpu_set_pc(cpu, pc_set);

    uint32_t inst = change_endianness(gen_instruction(opcode, 8, opc2, 3));
    mips_mem_write(ram, pc_set, 4, (uint8_t*)&inst);
    inst = change_endianness(gen_instruction(8, 9, 10, 0, ADDU));
    mips_mem_write(ram, pc_set+4, 4, (uint8_t*)&inst);
    inst = change_endianness(gen_instruction(8, 12, 11, 0, ADDU));
    mips_mem_write(ram, pc_set+16, 4, (uint8_t*)&inst);
    inst = change_endianness(gen_instruction(8, 13, 10, 0, ADDU));
    mips_mem_write(ram, pc_set+8, 4, (uint8_t*)&inst);

    mips_cpu_set_register(cpu, 8, a);
    mips_cpu_set_register(cpu, 9, b);
    mips_cpu_set_register(cpu, 12, 30);
    mips_cpu_set_register(cpu, 13, 100);

    mips_cpu_step(cpu);
    mips_cpu_step(cpu);
    mips_cpu_step(cpu);

    mips_cpu_get_register(cpu, 10, &reg10);
    mips_cpu_get_register(cpu, 11, &reg11);

    if(l == 1) {
        mips_cpu_get_register(cpu, 31, &reg31);
        if(reg31 == pc_set+8) {
            pass = 1;
        }
    } else {
        pass = 1;
    }

    if(reg10 == a+b && reg11 == a+30 && pass) {
        passed = 1;
    } else {
        passed = 0;
    }
    
    return passed;
}

int test_jump(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode) {
    int passed = 0;
    uint32_t inst;

    mips_cpu_reset(cpu);

    if(opcode == 2 || opcode == 3) {
        inst = change_endianness(gen_instruction(opcode, 0x20));
    }

    return passed;
}

int test_load(mips_mem_h ram, mips_cpu_h cpu, uint32_t opcode, uint8_t num) {
    int passed = 0;
    uint32_t res;

    mips_cpu_reset(cpu);

    uint32_t inst = change_endianness(gen_instruction(opcode, 8, 9, 4));

    mips_mem_write(ram, 0, 4, (uint8_t*)&inst);
    mips_mem_write(ram, 0x30, 1, &num);

    mips_cpu_set_register(cpu, 8, 0x2c);

    mips_cpu_step(cpu);
    
    mips_cpu_get_register(cpu, 9, &res);

    if((res == (uint32_t)(int8_t)num && opcode == LB) || (res == (uint32_t)num && opcode == LBU)) {
        passed = 1;
    }

    return passed;
}

