#include "../../include/mips.h"
#include "mips_cpu_ymh15.hpp"

struct mips_cpu_impl {
    mips_mem_h mem;
    uint32_t regs[32];
    // lo and hi registers
    uint32_t lo, hi;
    // program counter
    uint32_t pc;
    // next pc used for branches and set the pc correctly
    uint32_t next_pc;
    // delay slot that makes branches work as expected
    uint32_t delay_slot;

    // sets the debug level so that you can print more information
    int debug_level;
    FILE *debug_type;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem) {
    // creates new instance of a cpu explicitly
    mips_cpu_impl* state = new mips_cpu_impl;

    // reset all the variables
    state->mem = mem;
    state->pc = 0;
    state->next_pc = state->pc + 4;
    state->delay_slot = 0;
    state->lo = 0;
    state->hi = 0;
    
    state->debug_level = 0;
    state->debug_type = NULL;

    // sets all registers to 0
    for(int i = 0; i < 32; ++i) {
        state->regs[i] = 0;
    }

    return state;
}

mips_error mips_cpu_reset(mips_cpu_h state) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }

    // resets both program counters
    state->pc = 0;
    state->next_pc = state->pc + 4;
    state->delay_slot = 0;
    state->hi = 0;
    state->lo = 0;

    // and registers
    for(int i = 0; i < 32; ++i) {
        state->regs[i] = 0;
    }

    return mips_Success;
}

mips_error mips_cpu_get_register(mips_cpu_h state, unsigned index,
                                 uint32_t *value) {
    if(!state || !value || index > 31 || index == 0) {
        return mips_ErrorInvalidArgument;
    }

    // get the state of a current register by using the provided pointer;
    *value = state->regs[index];
    
    return mips_Success;
}

mips_error mips_cpu_set_register(mips_cpu_h state, unsigned index,
                                 uint32_t value) {
    if(!state || index > 31 || index == 0) {
        return mips_ErrorInvalidArgument;
    }

    // set reg state
    state->regs[index] = value;
    
    return mips_Success;
}


mips_error mips_cpu_set_pc(mips_cpu_h state, uint32_t pc) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }

    // set the pc and next_pc to the right values
    state->pc = pc;
    state->next_pc = state->pc+4;

    return mips_Success;
}

mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc) {
    if(!state || !pc) {
        return mips_ErrorInvalidArgument;
    }

    // get the value of the current pc
    *pc = state->pc;

    return mips_Success;
}

// This function executes the code that is found in memory at the address
// of the program counter
mips_error mips_cpu_step(mips_cpu_h state) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }

    // gets the instruction from memory
    uint32_t inst;
    mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&inst);
    
    // change the instruction back from big endian as it was read as little endian
    inst = (inst<<24) | ((inst>>8)&0xff00) | ((inst<<8)&0xff0000) | (inst>>24);

    // it then executes the instruction by decoding it first and returns
    // the state of the instruction
    mips_error mips_err = exec_instruction(state, inst);


    if(state->debug_level > 0) {
        fprintf(state->debug_type, "pc: %d\tpc_next: %d\tinst: %#10x\tdelay_slot: %d\n", state->pc, state->next_pc, inst, state->delay_slot);
    }

    // if the debug level is 2 or higher it will print the register state
    if(state->debug_level > 1) {
        fprintf(state->debug_type, "\nCPU register state:\n");
        for(unsigned i = 0; i < 32; ++i) {
            fprintf(state->debug_type, "R%d:\t%#10x\n", i,
                    state->regs[i]);
        }
        fprintf(state->debug_type, "\n\n");
    }

    if(state->debug_level > 2) {
        char c = getchar();
    }
    
    // it then updates the pc to next_pc which could have been changed
    // by a branch, it only does this if it is not in a delay slot   
    if(!state->delay_slot) {
        state->pc = state->next_pc;
        state->next_pc += 4;
    } else {
        state->pc += 4;
        --state->delay_slot;
    }

    // returns the operation error such as arithmetic overflow
    return mips_err;
}

mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level,
                                    FILE *dest) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }

    // just sets the cpu values for the debug level
    state->debug_level = level;
    state->debug_type = dest;
    
    return mips_Success;
}

void mips_cpu_free(mips_cpu_h state) {
    // deletes the state pointer of mips_cpu_impl* type
    if(state) {
        delete state;
    }
}

// The following functions are not present in the api as they are
// functions defined in a private header file

mips_error exec_instruction(mips_cpu_h state, uint32_t inst) {
    // creates var that holds all the information of the instruction
    uint32_t var[8];

    // prints pc and instruction every clock cycle
    
    for(int i = 0; i < 8; ++i) {
        var[i] = 0;
    }

    // decodes the instruction

    // if first 6 bits are 0 then it is a R instruction
    if(((inst >> 26)&0x3f) == 0) {
        // R Type: set all necessary variables
        var[REG_S] = inst >> 21;
        var[REG_T] = (inst >> 16)&0x1f;
        var[REG_D] = (inst >> 11)&0x1f;
        var[SHIFT] = (inst >> 6)&0x1f;
        var[FUNC] = inst&0x3f;

        // execute R instruction to perform operation
        return exec_R(state, var);
    } else if(((inst >> 26)&0x3f) < 4 && ((inst >> 26)&0x3f) != 1) {    // as opcode is < 4
        var[OPCODE] = inst >> 26;           // it has to be J type
        var[MEM] = inst&0x3ffffff;
        return exec_J(state, var);
    } else {                                // otherwise it is an I type
        var[OPCODE] = inst >> 26;      
        var[REG_S] = (inst >> 21)&0x1f;
        var[REG_D] = (inst >> 16)&0x1f;
        var[IMM] = inst&0xffff;
        if((var[IMM]&0x8000) == 0x8000) {
            var[IMM] = (var[IMM]|0xffff0000);
        }
        
        return exec_I(state, var);
    }
}

mips_error exec_R(mips_cpu_h state, uint32_t var[8]) {
    // if the function code is between 0x1f and 0x24 then it is addition
    // or subtraction
    if((var[FUNC]&0xf0) == 0x20 && (var[FUNC]&0xf) < 4) {
        // calls add with -1 if it is a subtractin and 1 if it is addition
        return add_sub(state, var, ((int32_t)-(var[FUNC]&0xf)/2)*2+1, 0);
        
    } else if((var[FUNC]&0xf0) == 0x20 && (var[FUNC]&0xf) < 7) {
        // else if the number is between 0x23 and 0x27 which means it
        // is a bitwise operation
        return bitwise(state, var, 0);
        
    } else if(var[FUNC] == 8) {
        // jr
        return jump(state, var, 0);
        
    } else if(var[FUNC] == 9) {
        // jalr
        return jump(state, var, 1);
        
    } else if(var[FUNC] >= 0x10 && var[FUNC] <= 0x13) {
        // mfhi mthi mflo mtlo
        return move(state, var);
        
    } else if(var[FUNC] >= 0x18 && var[FUNC] <= 0x1b) {
        // mult multu div divu
        return mult_div(state, var);
        
    } else {
        // otherwise return that it was an invalid instruction
        return mips_ExceptionInvalidInstruction;
    }
}

mips_error exec_J(mips_cpu_h state, uint32_t var[8]) {
    switch(var[OPCODE]) {
    case 0:
    case 2: // j
        return jump(state, var, 0);
    case 3: // jal
        return jump(state, var, 1);
    default:
        return mips_ExceptionInvalidInstruction;
    }
}

mips_error exec_I(mips_cpu_h state, uint32_t var[8]) {
    switch(var[OPCODE]) {
    case 0x1: // bgez, bgezal, bltz, bltzal
        return branch(state, var);
    case 0x4: // beq
        return branch(state, var);
    case 0x5: // bne
        return branch(state, var);
    case 0x6: // blez
        return branch(state, var); 
    case 0x7: // bgtz
        return branch(state, var);
    case 0x8: // addi
        return add_sub(state, var, 1, 1);
    case 0x9: // addiu
        return add_sub(state, var, 1, 2);
    case 0xc: // andi
        return bitwise(state, var, 1);
    case 0xd: // ori
        return bitwise(state, var, 1);
    case 0xe: // xori
        return bitwise(state, var, 1);
    case 0xf: // lui
        return load(state, var);
    case 0x20: // lb
        return load(state, var);
    case 0x21: // lh
        return load(state, var);
    case 0x22: // lwl
        return load(state, var);
    case 0x23: // lw
        return load(state, var);
    case 0x24: // lbu
        return load(state, var);
    case 0x25: // lhu
        return load(state, var);
    case 0x26: // lwr
        return load(state, var);
    case 0x28: // sb
        return store(state, var);
    case 0x29: // sh
        return store(state, var);
    case 0x2b: // sw
        return store(state, var);
    default:
        return mips_ExceptionInvalidInstruction;
    }
}

mips_error add_sub(mips_cpu_h state, uint32_t var[8], int32_t add_sub,
                   int32_t imm) {
    // sets initial values of the registers as local variables
    int32_t reg_s, reg_t, reg_d;

    // set the locals to the correct value from the register
    mips_cpu_get_register(state, var[REG_S], (uint32_t*)&reg_s);
    if(imm > 0) {
        reg_t = (int32_t)var[IMM];
    } else {
        mips_cpu_get_register(state, var[REG_T], (uint32_t*)&reg_t);
    }

    // perform the addition or subtraction
    reg_d = reg_s + add_sub*reg_t;

    // check for overflow conditions and if it is an unsigned operation
    if((var[FUNC]&0x1) == 1 || imm > 1 || !((reg_s > 0 && add_sub*reg_t > 0 && reg_d < 0) || (reg_s < 0 && add_sub*reg_t < 0 && reg_d > 0))) {

        // sets the destination register to the right answer
        mips_cpu_set_register(state, var[REG_D], (uint32_t)reg_d);
        return mips_Success;
    }
    
    return mips_ExceptionArithmeticOverflow;
}

mips_error bitwise(mips_cpu_h state, uint32_t var[8], unsigned imm) {
    // selects the right bitwise operation to perform then sets the register to the right value
    uint32_t reg_s, reg_t, reg_d;
    
    mips_cpu_get_register(state, var[REG_S], &reg_s);
    if(imm == 0) {
        mips_cpu_get_register(state, var[REG_T], &reg_t);
    } else {
        reg_t = var[IMM];
    }

    // performs correct bitwise operation and sets register;
    if((var[FUNC]&0xf) == 4 || var[OPCODE] == 0xc) reg_d = reg_s & reg_t;
    else if((var[FUNC]&0xf) == 5 || var[OPCODE] == 0xd) reg_d = reg_s | reg_t;
    else reg_d = reg_s ^ reg_t;

    // set register to the answer
    mips_cpu_set_register(state, var[REG_D], reg_d);
    
    return mips_Success;
}

mips_error branch(mips_cpu_h state, uint32_t var[8]) {
    if(var[OPCODE] == 1 && (var[REG_D] == 0x11 || var[REG_D] == 0x10)) {
        state->regs[31] = state->pc+8;
    }

    if((state->regs[var[REG_S]] == state->regs[var[REG_D]] && var[OPCODE] == 4) || // bez
       (state->regs[var[REG_S]] != state->regs[var[REG_D]] && var[OPCODE] == 5) || // bne
       (((int32_t)state->regs[var[REG_S]] >= 0) && (var[OPCODE] == 1) && (var[REG_D]&1) == 1) || // bgez / bgezal
       ((int32_t)state->regs[var[REG_S]] < 0 && var[OPCODE] == 1 && (var[REG_D]&1) == 0) || // bltz / bltzal
       ((int32_t)state->regs[var[REG_S]] > 0 && var[OPCODE] == 7 && (var[REG_D]&1) == 0) || // bgtz
       ((int32_t)state->regs[var[REG_S]] <= 0 && var[OPCODE] == 6 && (var[REG_D]&1) == 0)) { // blez
        
        state->next_pc += (var[IMM]<<2);
        state->delay_slot += 1;
    }
    
    return mips_Success;
}

mips_error jump(mips_cpu_h state, uint32_t var[8], uint8_t link) {
    uint8_t reg_d;
    uint32_t jump_loc;
    if(var[FUNC] == 9 || var[FUNC] == 8) {
        jump_loc = state->regs[var[REG_S]];
        reg_d = var[REG_D];
    } else {
        jump_loc = var[MEM]<<2;
        reg_d = 31;
    }
    
    if(link) {
        state->regs[reg_d] = state->pc+8;
    }
    
    state->next_pc = jump_loc;
    state->delay_slot += 1;

    return mips_Success;
}

mips_error load(mips_cpu_h state, uint32_t var[8]) {
    uint32_t addr;
    uint8_t mem_byte;
    uint16_t mem_halfword;
    uint32_t mem_word;
    
    addr = state->regs[var[REG_S]] + var[IMM];

    if(var[OPCODE] == 0x24 || var[OPCODE] == 0x20) { // lb lbu
        mips_mem_read(state->mem, addr, 1, &mem_byte);
    } else if(var[OPCODE] == 0x21 || var[OPCODE] == 0x25) { // lh lhu
        if((addr&0b1) == 1) {
            return mips_ExceptionInvalidAddress;
        }
        mips_mem_read(state->mem, addr, 2, (uint8_t*)&mem_halfword);
        mem_halfword = mem_halfword>>8 | mem_halfword<<8;
    } else if(var[OPCODE] == 0x23 || var[OPCODE] == 0x22) { // lw lwl
        if((addr&0b1) == 1 || (addr&0b10)>>1 == 1) {
            return mips_ExceptionInvalidAddress;
        }
        mips_mem_read(state->mem, addr, 4, (uint8_t*)&mem_word);
        mem_word = (mem_word<<24) | ((mem_word>>8)&0xff00) | ((mem_word<<8)&0xff0000) | (mem_word>>24);
    } else if(var[OPCODE] == 0x26) { // lwr
        if((addr&0b1) == 0 || (addr&0b10)>>1 == 0) {
            return mips_ExceptionInvalidAddress;
        }
        mips_mem_read(state->mem, addr-3, 4, (uint8_t*)&mem_word);
        mem_word = (mem_word<<24) | ((mem_word>>8)&0xff00) | ((mem_word<<8)&0xff0000) | (mem_word>>24);
    }

    switch(var[OPCODE]) {
    case 0xf:
        state->regs[var[REG_D]] = var[IMM]<<16;
        return mips_Success;
    case 0x20:
        state->regs[var[REG_D]] = (uint32_t)(int8_t)mem_byte;
        return mips_Success;
    case 0x21:
        state->regs[var[REG_D]] = (uint32_t)(int16_t)mem_halfword;
        return mips_Success;
    case 0x22:
        state->regs[var[REG_D]] = (mem_word&0xffff0000)|(state->regs[var[REG_D]]&0xffff);
        return mips_Success;
    case 0x23:
        state->regs[var[REG_D]] = mem_word;
        return mips_Success;
    case 0x24:
        state->regs[var[REG_D]] = (uint32_t)mem_byte;
        return mips_Success;
    case 0x25:
        state->regs[var[REG_D]] = (uint32_t)mem_halfword;
        return mips_Success;
    case 0x26:
        state->regs[var[REG_D]] = (state->regs[var[REG_D]]&0xffff0000)|(mem_word&0xffff);
        return mips_Success;
    default:
        return mips_ExceptionInvalidInstruction;
    }
}

mips_error store(mips_cpu_h state, uint32_t var[8]) {
    uint32_t addr;
    uint32_t word;
    uint16_t half_word;
    uint8_t byte;
    
    addr = state->regs[var[REG_S]] + var[IMM];
    word = state->regs[var[REG_D]];
    half_word = (uint16_t)state->regs[var[REG_D]];
    byte = (uint8_t)state->regs[var[REG_D]];
    
    if(var[OPCODE] == 0x28) {
        mips_mem_write(state->mem, addr, 1, &byte);
    } else if(var[OPCODE] == 0x29) {
        if((addr&0b1) == 1) {
            return mips_ExceptionInvalidAddress;
        }
        uint16_t tmp = half_word << 8 | half_word >> 8;
        mips_mem_write(state->mem, addr, 2, (uint8_t*)&tmp);
    } else if(var[OPCODE] == 0x2b) {
        if((addr&0b1) == 1 || (addr&0b10)>>1 == 1) {
            return mips_ExceptionInvalidAddress;
        }
        uint32_t tmp = word<<24 | ((word>>8)&0xff00) | ((word<<8)&0xff0000) | word>>24;
        mips_mem_write(state->mem, addr, 4, (uint8_t*)&tmp);
    } else {
        return mips_ExceptionInvalidInstruction;
    }
    return mips_Success;
}

mips_error move(mips_cpu_h state, uint32_t var[8]) {
    switch(var[FUNC]) {
    case 0x10:
        return mips_cpu_set_register(state, var[REG_D], state->hi);
    case 0x11:
        return mips_cpu_get_register(state, var[REG_D], &state->hi);
    case 0x12:
        return mips_cpu_set_register(state, var[REG_D], state->lo);
    case 0x13:
        return mips_cpu_get_register(state, var[REG_D], &state->lo);
    default:
        return mips_ExceptionInvalidInstruction;
    }
    return mips_Success;
}

mips_error mult_div(mips_cpu_h state, uint32_t var[8]) {
    uint64_t unsigned_result, signed_result;
    switch(var[FUNC]) {
    case 0x18: // mult
        signed_result = ((int64_t)(int32_t)state->regs[var[REG_S]])*((int64_t)(int32_t)state->regs[var[REG_T]]);
        state->lo = (uint32_t)signed_result;
        state->hi = (uint32_t)(signed_result>>32);
        return mips_Success;
        
    case 0x19: // multu
        unsigned_result = ((uint64_t)state->regs[var[REG_S]])*((uint64_t)state->regs[var[REG_T]]);
        state->lo = (uint32_t)unsigned_result;
        state->hi = (uint32_t)(unsigned_result>>32);
        return mips_Success;
        
    case 0x1a: // div
        if(var[REG_T] == 0) {
            state->lo = 0;
            state->hi = 0;
            return mips_Success;
        }
        
        state->lo = ((int32_t)state->regs[var[REG_S]])/((int32_t)state->regs[var[REG_T]]);
        state->hi = ((int32_t)state->regs[var[REG_S]])%((int32_t)state->regs[var[REG_T]]);
        return mips_Success;
        
    case 0x1b: // divu
        if(var[REG_T] == 0) {
            state->lo = 0;
            state->hi = 0;
            return mips_Success;
        }
        
        state->lo = (state->regs[var[REG_S]])/(state->regs[var[REG_T]]);
        state->hi = (state->regs[var[REG_S]])%(state->regs[var[REG_T]]);
        return mips_Success;
        
    default:
        return mips_ExceptionInvalidInstruction;
    }
}

mips_error shift(mips_cpu_h state, uint32_t var[8]) {
    if(var[FUNC] == 0 && var[OPCODE] == 0) {
        state->regs[var[REG_D]] = state->regs[var[REG_T]] << var[SHIFT];
    } else if(var[FUNC] == 4) {
        state->regs[var[REG_D]] = state->regs[var[REG_T]] << state->regs[var[REG_S]];
    } else if(var[FUNC] == 2/*TODO*/);
    return mips_Success;
}
