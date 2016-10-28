#include "test_mips_ymh15.hpp"

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

void change_endianness(uint32_t &inst) {
    inst = (inst << 24 | ((inst << 8)&0xff0000) |
            ((inst >> 8)&0xff00) |inst >> 24);
}

int check_reg(mips_cpu_h state, uint8_t reg_addr, uint32_t check_value) {
    uint32_t reg;

    if(reg_addr != 255)
        mips_cpu_get_register(state, reg_addr, &reg);
    else
        mips_cpu_get_pc(state, &reg);

    if(reg == check_value)
        return 1;

    printf("Value of reg%d: %#10x\tExpected value: %#10x\n", reg_addr, reg, check_value);
    return 0;
}

int check_error(mips_error err, mips_error expected_err) {
    if(err == expected_err) {
        return 1;
    }

    printf("Received error: %#6x\tExpected error: %#6x\n", err, expected_err);
    return 0;
}

mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t src1, uint32_t src2, uint32_t dest, uint32_t shift, uint32_t function) {
    uint32_t inst;
    mips_error err;

    rand_reg(state);
    inst = gen_instruction(src1, src2, dest, shift, function);
    change_endianness(inst);
    err = mips_mem_write(mem, mem_location, 4, (uint8_t*)&inst);
    return err;
}

mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t opcode, uint32_t src, uint32_t dest, uint32_t Astart) {
    uint32_t inst;
    mips_error err;

    rand_reg(state);
    inst = gen_instruction(opcode, src, dest, Astart);
    change_endianness(inst);
    err = mips_mem_write(mem, mem_location, 4, (uint8_t*)&inst);
    if(err != mips_Success) {
        printf("couln't write...\n");
    }
    return err;
}

mips_error set_instruction(mips_mem_h mem, mips_cpu_h state, uint32_t mem_location, uint32_t opcode, uint32_t memory) {
    uint32_t inst;
    mips_error err;

    rand_reg(state);
    inst = gen_instruction(opcode, memory);
    change_endianness(inst);
    err = mips_mem_write(mem, mem_location, 4, (uint8_t*)&inst);
    return err;

}

mips_error rand_reg(mips_cpu_h state) {
    mips_error err;
    for(unsigned i = 1; i < 32; ++i) {
        err = mips_cpu_set_register(state, i, rand()%0xffffffff);
        if(err != mips_Success)
            return err;
    }
    return mips_Success;
}
