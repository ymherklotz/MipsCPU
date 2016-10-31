#include "test_mips_ymh15.hpp"

int main(int argc, char** argv) {
    mips_mem_h test_ram = mips_mem_create_ram(0x20000000);
    mips_cpu_h test_cpu = mips_cpu_create(test_ram);
    int testId;
    mips_error err;
    uint8_t byte;
    uint16_t halfword;
    uint32_t word;

    srand(time(NULL));

    if(argc == 2) {
        mips_cpu_set_debug_level(test_cpu, argv[1][0]-'0', stdout);
    } else {
        mips_cpu_set_debug_level(test_cpu, 0, NULL);
    }

    mips_test_begin_suite();

    testId = mips_test_begin_test("<INTERNAL>");
    mips_cpu_set_register(test_cpu, 0, 0xf28ba9);
    mips_test_end_test(testId, check_reg(test_cpu, 0, 0), "Testing if reg 0 = 0");

    testId = mips_test_begin_test("<INTERNAL>");
    mips_cpu_set_register(test_cpu, 27, 0xf28ba9);
    mips_test_end_test(testId, check_reg(test_cpu, 27, 0xf28ba9), "Check if set register is actually set");

    testId = mips_test_begin_test("<INTERNAL>");
    mips_cpu_set_pc(test_cpu, 0x20);
    mips_test_end_test(testId, check_reg(test_cpu, 255, 0x20), "Check if pc can be set and read");

    testId = mips_test_begin_test("ADD");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADD);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xfb29ce8);
    mips_cpu_set_register(test_cpu, 10, 0x873bef);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x1039d8d7), "Check basic add functionality");
    
    testId = mips_test_begin_test("ADD");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADD);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0x7fffffff);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_ExceptionArithmeticOverflow), "Checking if add handles overflow correctly by setting exception");
    
    testId = mips_test_begin_test("ADD");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADD);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xfa139c);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0x7fffffff);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xfa139c), "Checking if add handles overflow correctly by not setting register");
    
    testId = mips_test_begin_test("ADDI");
    set_instruction(test_ram, test_cpu, 0, ADDI, 9, 8, 0x29f2);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x194a2b);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x19741d), "testing basic functionality of ADDI");
    
    testId = mips_test_begin_test("ADDI");
    set_instruction(test_ram, test_cpu, 0, ADDI, 9, 8, 0xf293);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x193);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xfffff426), "Check if immediates are handled correctly by ADDI");

    testId = mips_test_begin_test("ADDIU");
    set_instruction(test_ram, test_cpu, 0, ADDIU, 9, 8, 0x2f9e);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x3f9e72b);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x3fa16c9), "check basic functionality off ADDIU");

    testId = mips_test_begin_test("ADDU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xeb2931);
    mips_cpu_set_register(test_cpu, 10, 0x983be2);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x1836513), "Checking basic functionality of ADDU");

    testId = mips_test_begin_test("ADDU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0x7fffffff);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_Success), "Checking if ADDU handles overflow correctly by not setting exception");
    
    testId = mips_test_begin_test("ADDU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0x7fffffff);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xFFFFFFFE), "Checking if ADDU handles overflow correctly by setting register");

    testId = mips_test_begin_test("AND");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, AND);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf1e8ba2c);
    mips_cpu_set_register(test_cpu, 10, 0xff8cf2e9);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xF188B228), "Testing functionality of AND");

    testId = mips_test_begin_test("ANDI");
    set_instruction(test_ram, test_cpu, 0, ANDI, 9, 8, 0xf293);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xff928228);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x8200), "Testing functionality of ANDI");

    testId = mips_test_begin_test("BEQ");
    set_instruction(test_ram, test_cpu, 0, BEQ, 9, 8, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing basic functionality of BEQ");

    testId = mips_test_begin_test("BEQ");
    set_instruction(test_ram, test_cpu, 0x200, BEQ, 9, 8, 0xff94);
    set_instruction(test_ram, test_cpu, 0x204, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x208, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0x200);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing negative branch with BEQ");

    testId = mips_test_begin_test("BGEZ");
    set_instruction(test_ram, test_cpu, 0, BGEZ, 8, 1, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing basic functionality of BGEZ");

    testId = mips_test_begin_test("BGEZAL");
    set_instruction(test_ram, test_cpu, 0, BGEZAL, 8, 0x11, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 31, 8), "check if the link works on BGEZAL");

    testId = mips_test_begin_test("BGTZ");
    set_instruction(test_ram, test_cpu, 0, BGTZ, 8, 0, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing basic functionality of BGTZ");

    testId = mips_test_begin_test("BLEZ");
    set_instruction(test_ram, test_cpu, 0, BLEZ, 8, 0, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 2), "Testing functionality of BLEZ");

    testId = mips_test_begin_test("BLTZ");
    set_instruction(test_ram, test_cpu, 0, BLTZ, 7, 0, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 7, 0xffffffff);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing functionality of BLTZ");

    testId = mips_test_begin_test("BLTZAL");
    set_instruction(test_ram, test_cpu, 0, BLTZAL, 7, 0x10, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 7, 0xffffffff);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 31, 8), "Testing if BLTZAL links");

    testId = mips_test_begin_test("BNE");
    set_instruction(test_ram, test_cpu, 0, BNE, 9, 7, 0x1f94);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x7e54, 10, 9, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 7, 3);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Testing BNE");

    testId = mips_test_begin_test("DIV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, DIV);
    set_instruction(test_ram, test_cpu, 4, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -39);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, -9), "Test DIV Quotient");

    // DIV
    testId = mips_test_begin_test("DIV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, DIV);
    set_instruction(test_ram, test_cpu, 4, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -39);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, -3), "Test DIV remainder");

    // DIVU
    testId = mips_test_begin_test("DIVU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, DIVU);
    set_instruction(test_ram, test_cpu, 4, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -39);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x3FFFFFF6), "Test DIVU Quotient");

    testId = mips_test_begin_test("DIVU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, DIVU);
    set_instruction(test_ram, test_cpu, 4, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -39);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 1), "Test DIVU remainder");

    // J
    testId = mips_test_begin_test("J");
    mips_cpu_reset(test_cpu);
    set_instruction(test_ram, test_cpu, 0, J, 0x298c2);
    set_instruction(test_ram, test_cpu, 4, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xa6308, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Checking basic jump");

    testId = mips_test_begin_test("J");
    mips_cpu_reset(test_cpu);
    set_instruction(test_ram, test_cpu, 0x10000000, J, 0x24);
    set_instruction(test_ram, test_cpu, 0x10000004, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x10000008, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0x10000090, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 0x10000000);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Test if upper 4 bits of the pc stay the same");

    // JAL
    testId = mips_test_begin_test("JAL");
    mips_cpu_reset(test_cpu);
    set_instruction(test_ram, test_cpu, 4, JAL, 0x298c2);
    set_instruction(test_ram, test_cpu, 8, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xa6308, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 4);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 31, 0xc), "checking if J links correctly");

    // JALR
    testId = mips_test_begin_test("JALR");
    set_instruction(test_ram, test_cpu, 4, 7, 0, 31, 0, JALR);
    set_instruction(test_ram, test_cpu, 8, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xa6308, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 4);
    mips_cpu_set_register(test_cpu, 7, 0xa6308);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 31, 0xc), "Checking if JALR links correctly");

    // JR
    testId = mips_test_begin_test("JR");
    set_instruction(test_ram, test_cpu, 4, 7, 0, 0, 0, JR);
    set_instruction(test_ram, test_cpu, 8, 9, 8, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xa6308, 10, 8, 10, 0, ADDU);
    mips_cpu_set_pc(test_cpu, 4);
    mips_cpu_set_register(test_cpu, 7, 0xa6308);
    mips_cpu_set_register(test_cpu, 8, 2);
    mips_cpu_set_register(test_cpu, 9, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 10, 6), "Checking if JR jumps correctly");

    // LB
    testId = mips_test_begin_test("LB");
    set_instruction(test_ram, test_cpu, 0, LB, 9, 8, 0xffff);
    byte = 0xf9;
    mips_mem_write(test_ram, 0x30000, 1, &byte);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xfffffff9), "Checking basic functionality of LB");

    // LBU
    testId = mips_test_begin_test("LBU");
    set_instruction(test_ram, test_cpu, 0, LBU, 9, 8, 0x1);
    byte = 0xf9;
    mips_mem_write(test_ram, 0x2ffff, 1, &byte);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x2fffe);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xf9), "Checking basic functionality of LBU");

    // LH
    testId = mips_test_begin_test("LH");
    set_instruction(test_ram, test_cpu, 0, LH, 9, 8, 0xffff);
    halfword = 0xf9ff;
    mips_mem_write(test_ram, 0x30000, 2, (uint8_t*)&halfword);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xfffffff9), "Testing basic LH");

    // LHU
    testId = mips_test_begin_test("LHU");
    set_instruction(test_ram, test_cpu, 0, LHU, 9, 8, 0xffff);
    halfword = 0xf9ff;
    mips_mem_write(test_ram, 0x30000, 2, (uint8_t*)&halfword);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xfff9), "Testing basic LHU");

    // LUI
    testId = mips_test_begin_test("LUI");
    set_instruction(test_ram, test_cpu, 0, LUI, 0, 8, 0xf9f2);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xf9f20000), "Testing basic LUI");

    // LW
    testId = mips_test_begin_test("LW");
    set_instruction(test_ram, test_cpu, 0, LW, 9, 8, 0xffff);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xf82e9b2f), "Testing basic LW");

    testId = mips_test_begin_test("LW");
    set_instruction(test_ram, test_cpu, 0, LW, 9, 8, 0xffff);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x30002);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_ExceptionInvalidAlignment), "Testing basic LW");
    
    // LWL
    testId = mips_test_begin_test("LWL");
    set_instruction(test_ram, test_cpu, 0, LWL, 9, 8, 0xffff);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xa8b234e2);
    mips_cpu_set_register(test_cpu, 9, 0x30002);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x2e9b2fe2), "Checking basic LWL");

    testId = mips_test_begin_test("LWL");
    set_instruction(test_ram, test_cpu, 0, LWL, 9, 8, 0xffff);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xa8b234e2);
    mips_cpu_set_register(test_cpu, 9, 0x30003);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x9b2f34e2), "Checking other LWL");

    // LWR
    testId = mips_test_begin_test("LWR");
    set_instruction(test_ram, test_cpu, 0, LWR, 9, 8, 0x1);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xa8b234e2);
    mips_cpu_set_register(test_cpu, 9, 0x30000);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xa8b2f82e), "Testing basic LWR");

    testId = mips_test_begin_test("LWR");
    set_instruction(test_ram, test_cpu, 0, LWR, 9, 8, 2);
    word = 0xf82e9b2f;
    change_endianness(word);
    mips_mem_write(test_ram, 0x30000, 4, (uint8_t*)&word);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xa8b234e2);
    mips_cpu_set_register(test_cpu, 9, 0x30000);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xa8f82e9b), "Testing LWR with different offset");

    // MFHI
    testId = mips_test_begin_test("MFHI");
    set_instruction(test_ram, test_cpu, 0, 9, 0, 0, 0, MTHI);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x20);
    mips_cpu_set_register(test_cpu, 10, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x20), "Checking basics of MTHI and MFHI");

    // MFLO
    testId = mips_test_begin_test("MFLO");
    set_instruction(test_ram, test_cpu, 0, 9, 0, 0, 0, MTLO);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x20);
    mips_cpu_set_register(test_cpu, 10, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x20), "Checking basics of MTLO and MFLO");

    // MTHI
    testId = mips_test_begin_test("MTHI");
    set_instruction(test_ram, test_cpu, 0, 9, 0, 0, 0, MTHI);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x20);
    mips_cpu_set_register(test_cpu, 10, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x20), "Checking basics of MTHI and MFHI");

    // MTLO
    testId = mips_test_begin_test("MTLO");
    set_instruction(test_ram, test_cpu, 0, 9, 0, 0, 0, MTLO);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x20);
    mips_cpu_set_register(test_cpu, 10, 2);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x20), "Checking basics of MTLO AND MFLO");

    // MULT
    testId = mips_test_begin_test("MULT");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, MULT);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -139);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, -556), "Checking basics of MULT, LO");
    
    testId = mips_test_begin_test("MULT");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, MULT);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -139);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, -1), "Checking basics of MULT, HI");

    // MULTU
    testId = mips_test_begin_test("MULTU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, MULTU);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFLO);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -139);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xFFFFFDD4), "Checking basics of MULTU, LO");

    testId = mips_test_begin_test("MULTU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, MULTU);
    set_instruction(test_ram, test_cpu, 4, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 8, 10, 10, 10, 0, ADDU);
    set_instruction(test_ram, test_cpu, 0xc, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -139);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x3), "Checking basics of MULTU, HI");

    // OR
    testId = mips_test_begin_test("OR");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, OR);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf1e8ba2c);
    mips_cpu_set_register(test_cpu, 10, 0xff8cf2e9);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xFFECFAED), "Testing basic OR");

    // ORI
    testId = mips_test_begin_test("ORI");
    set_instruction(test_ram, test_cpu, 0, ORI, 9, 8, 0x2293);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xff928228);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xff92a2bb), "Testing basic ORI");

    // SB
    testId = mips_test_begin_test("SB");
    set_instruction(test_ram, test_cpu, 0, SB, 9, 8, 0xffff);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xff92822f);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_mem(test_ram, 0x30000, 1, 0x2f), "Testing basic SB");

    // SH
    testId = mips_test_begin_test("SH");
    set_instruction(test_ram, test_cpu, 0, SH, 9, 8, 1);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xff92822f);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_mem(test_ram, 0x30002, 2, 0x822f), "Test basic SH");

    testId = mips_test_begin_test("SH");
    set_instruction(test_ram, test_cpu, 0, SH, 9, 8, 1);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xff92822f);
    mips_cpu_set_register(test_cpu, 9, 0x30000);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_ExceptionInvalidAlignment), "Test alignment of SH");

    // SLL
    testId = mips_test_begin_test("SLL");
    set_instruction(test_ram, test_cpu, 0, 0, 9, 8, 4, SLL);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x293FEB10), "Testing basic SLL");

    // SLLV
    testId = mips_test_begin_test("SLLV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SLLV);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 4);
    mips_cpu_set_register(test_cpu, 10, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0x293FEB10), "Testing basic SLL");

    // SLT
    testId = mips_test_begin_test("SLT");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SLT);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -3);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 1), "Testing SLT with signed");

    // SLTI
    testId = mips_test_begin_test("SLTI");
    set_instruction(test_ram, test_cpu, 0, SLTI, 9, 8, 0xf953);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xfffff800);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 1), "Testing SLTI with negatives");

    // SLTUI
    testId = mips_test_begin_test("SLTIU");
    set_instruction(test_ram, test_cpu, 0, SLTIU, 9, 8, 0x11);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xffffffff);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0), "Testing SLTIU with a negative and positive which should give 0 in reg8");

    // SLTU
    testId = mips_test_begin_test("SLTU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SLTU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 3);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 1), "Testing basic SLTU");

    // SRA
    testId = mips_test_begin_test("SRA");
    set_instruction(test_ram, test_cpu, 0, 0, 9, 8, 4, SRA);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xff293feb), "Testing basic SRA");

    // SRAV
    testId = mips_test_begin_test("SRAV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SRAV);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 4);
    mips_cpu_set_register(test_cpu, 10, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xff293feb), "Testing basic SRAV");

    // SRL
    testId = mips_test_begin_test("SRL");
    set_instruction(test_ram, test_cpu, 0, 0, 9, 8, 4, SRL);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xf293feb), "Testing basic SRL");

    // SRLV
    testId = mips_test_begin_test("SRLV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SRLV);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 4);
    mips_cpu_set_register(test_cpu, 10, 0xf293feb1);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xf293feb), "Testing basic SRLV");

    // SUB
    testId = mips_test_begin_test("SUB");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUB);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xfb29ce8);
    mips_cpu_set_register(test_cpu, 10, 0x873bef);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xF2B60F9), "Testing basic SUB");

    testId = mips_test_begin_test("SUB");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUB);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0xa0000000);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_ExceptionArithmeticOverflow), "Testing basic SUB");

    testId = mips_test_begin_test("SUB");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUB);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0xa0000000);
    mips_cpu_get_register(test_cpu, 8, &word);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, word), "Testing if reg doesnt change when overflow happens");
    
    // SUBU
    testId = mips_test_begin_test("SUBU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUBU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0xa0000000);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_Success), "Testing if SUBU gives overflow");

    testId = mips_test_begin_test("SUBU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUBU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0x7fffffff);
    mips_cpu_set_register(test_cpu, 10, 0xa0000000);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xdfffffff), "Testing if SUBU gives overflow");

    testId = mips_test_begin_test("SUBU");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, SUBU);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xfb29ce8);
    mips_cpu_set_register(test_cpu, 10, 0x873bef);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xF2B60F9), "Testing if SUBU gives overflow");

    // SW
    testId = mips_test_begin_test("SW");
    set_instruction(test_ram, test_cpu, 0, SW, 9, 8, 0xffff);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xff92822f);
    mips_cpu_set_register(test_cpu, 9, 0x30001);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_mem(test_ram, 0x30000, 4, 0xff92822f), "Testing basic SW");

    testId = mips_test_begin_test("SW");
    set_instruction(test_ram, test_cpu, 0, SW, 9, 8, 0xffff);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 8, 0xff92822f);
    mips_cpu_set_register(test_cpu, 9, 0x30002);
    err = mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_error(err, mips_ExceptionInvalidAlignment), "Check alignment exception on SW");

    // XOR
    testId = mips_test_begin_test("XOR");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 8, 0, XOR);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xf1e8ba2c);
    mips_cpu_set_register(test_cpu, 10, 0xff8cf2e9);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xE6448C5), "Testing basic XOR");

    // XORI
    testId = mips_test_begin_test("XORI");
    set_instruction(test_ram, test_cpu, 0, XORI, 9, 8, 0x2293);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, 0xff928228);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, 0xff92a0bb), "Testing basic XORI");

    mips_test_end_suite();
    
    return 0;
}

