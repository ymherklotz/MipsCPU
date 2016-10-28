#include "test_mips_ymh15.hpp"

int main(int argc, char** argv) {
    mips_mem_h test_ram = mips_mem_create_ram(0x20000000);
    mips_cpu_h test_cpu = mips_cpu_create(test_ram);
    int testId;
    mips_error err;

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
    
    testId = mips_test_begin_test("DIV");
    set_instruction(test_ram, test_cpu, 0, 9, 10, 0, 0, DIV);
    set_instruction(test_ram, test_cpu, 4, 0, 0, 8, 0, MFHI);
    mips_cpu_set_pc(test_cpu, 0);
    mips_cpu_set_register(test_cpu, 9, -39);
    mips_cpu_set_register(test_cpu, 10, 4);
    mips_cpu_step(test_cpu);
    mips_cpu_step(test_cpu);
    mips_test_end_test(testId, check_reg(test_cpu, 8, -3), "Test DIV remainder");

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

    testId = mips_test_begin_test("LB");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LBU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LH");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LHU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LUI");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LW");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LWL");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("LWR");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MFHI");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MFLO");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MTHI");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MTLO");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MULT");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("MULTU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("OR");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("ORI");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SB");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SH");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLL");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLLV");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLT");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLTI");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLTIU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SLTU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SRA");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SRAV");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SRL");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SRLV");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SUB");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SUBU");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("SW");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("XOR");
    mips_test_end_test(testId, 0, "");

    testId = mips_test_begin_test("XORI");
    mips_test_end_test(testId, 0, "");

    mips_test_end_suite();
    
    return 0;
}
