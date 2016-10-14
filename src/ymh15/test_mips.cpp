#include "../../include/mips.h"
#include "test_mips_ymh15.hpp"

#include <iostream>

using namespace std;

int main() {
    mips_mem_h ram = mips_mem_create_ram(4096);
    mips_cpu_h cpu = mips_cpu_create(ram);
    
    uint32_t inst = gen_instruction(9, 10, 8, 0, 0x21);
    uint32_t inst2 = gen_instruction(12, 13, 11, 0, 0x20);
    uint32_t ans;
    uint32_t ans2;
    
    mips_mem_write(ram, 0, 4, (uint8_t*)&inst);
    mips_mem_write(ram, 4, 4, (uint8_t*)&inst2);

    mips_cpu_set_register(cpu, 9, 4);
    mips_cpu_set_register(cpu, 10, 5);
    mips_cpu_set_register(cpu, 12, 232);
    mips_cpu_set_register(cpu, 13, 2356);
    
    mips_cpu_step(cpu);
    mips_cpu_step(cpu);
    
    mips_cpu_get_register(cpu, 8, &ans);
    mips_cpu_get_register(cpu, 11, &ans2);
    
    if(!ans || !ans2) {
        printf("failed\n");
    } else {
        printf("4 + 5 = %d\n", ans);
        printf("232 + 2356 = %d\n", ans2);
    }
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
    inst = (inst << 24 | ((inst << 8)&0x00ff0000) |
            ((inst >> 8)&0x0000ff00) |inst >> 24);
    return inst;
}

void test_endian(mips_mem_h ram) {
    uint32_t address, length, data, data0, data1, data2, data3, dataf;
    address = 0;
    length = 4;
    data = 0x01234567;
    data0 = 0;
    data1 = 0;
    data2 = 0;
    data3 = 0;
    dataf = 0;

    printf("Data in: %#010x\n", data);

    //data = (data << 24 | ((data << 8)&0x00ff0000) |
    //        ((data >> 8)&0x0000ff00) |data >> 24);

    mips_mem_write(ram, address, length, (uint8_t*)&data);
    mips_mem_read(ram, address, 1, (uint8_t*)&data0);
    mips_mem_read(ram, address+1, 1, (uint8_t*)&data1);
    mips_mem_read(ram, address+2, 1, (uint8_t*)&data2);
    mips_mem_read(ram, address+3, 1, (uint8_t*)&data3);

    mips_mem_read(ram, address, 4, (uint8_t*)&dataf);
    
    cout << "Data at " << address << ": " << data0 << endl;
    cout << "Data at " << address+1 << ": " << data1 << endl;
    cout << "Data at " << address+2 << ": " << data2 << endl;
    cout << "Data at " << address+3 << ": " << data3 << endl;

    printf("Dataf: %#010x\n", dataf);
}

