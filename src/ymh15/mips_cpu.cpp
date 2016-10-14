#include "../../include/mips.h"
#include "mips_cpu_ymh15.hpp"

struct mips_cpu_impl {
    mips_mem_h mem;
    uint32_t regs[32];
    uint32_t pc;
    uint32_t overflow;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem) {
    mips_cpu_impl* new_mips_cpu = new mips_cpu_impl;
    
    new_mips_cpu->mem = mem;
    new_mips_cpu->pc = 0;
    new_mips_cpu->overflow = 0;
    
    for(int i = 0; i < 32; ++i) {
        new_mips_cpu->regs[i] = 0;
    }

    return new_mips_cpu;
}

mips_error mips_cpu_reset(mips_cpu_h state) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }
    
    state->pc = 0;
    state->overflow = 0;

    for(int i = 0; i < 32; ++i) {
        state->regs[i] = 0;
    }

    return mips_Success;
}

mips_error mips_cpu_get_register(mips_cpu_h state, unsigned index,
                                 uint32_t *value) {
    if(!state || !value) {
        return mips_ErrorInvalidArgument;
    }
    
    *value = state->regs[index];
    
    return mips_Success;
}

mips_error mips_cpu_set_register(mips_cpu_h state, unsigned index,
                                 uint32_t value) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }
    
    state->regs[index] = value;
    
    return mips_Success;
}


mips_error mips_cpu_set_pc(mips_cpu_h state, uint32_t pc) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }
    
    state->pc = pc;

    return mips_Success;
}

mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc) {
    if(!state || !pc) {
        return mips_ErrorInvalidArgument;
    }
    
    *pc = state->pc;

    return mips_Success;
}

mips_error mips_cpu_step(mips_cpu_h state) {
    if(!state) {
        return mips_ErrorInvalidArgument;
    }

    read_instruction(state);
    
    state->pc += 4;

    return mips_Success;
}

mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level,
                                    FILE *dest) {
    if(!state || !dest) {
        return mips_ErrorInvalidArgument;
    }
    
    //TODO
    
    return mips_Success;
}

void mips_cpu_free(mips_cpu_h state) {
    if(state) {
        delete state;
    }
}

mips_error read_instruction(mips_cpu_h state) {
    uint32_t inst;
    mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&inst);
    
    exec_instruction(state, inst);

    return mips_Success;
}

mips_error exec_instruction(mips_cpu_h state, uint32_t inst) {
    uint32_t var[8];
    for(int i = 0; i < 8; ++i) {
        var[i] = 0;
    }
    
    if(((inst >> 26)&0x3f) == 0) {
        // R Type
        var[REG_S] = inst >> 21;
        var[REG_T] = (inst >> 16)&0x1f;
        var[REG_D] = (inst >> 11)&0x1f;
        var[SHIFT] = (inst >> 6)&0x1f;
        var[FUNC] = inst&0x3f;
        
        exec_R(state, var);
    } else if(((inst >> 26)&0x3f) < 4) {
        var[OPCODE] = inst >> 26;
        var[REG_S] = (inst >> 21)&0x1f;
        var[REG_D] = (inst >> 16)&0x1f;
        var[IMM] = inst&0xffff;
        
        exec_J(state, var);
    } else {
        var[OPCODE] = inst >> 26;
        var[MEM] = inst&0x3ffffff;
        
        exec_I(state, var);
    }

    return mips_Success;
}

void exec_R(mips_cpu_h state, uint32_t var[8]) {
    if((var[FUNC]&0xf0) == 0x20 && (var[FUNC]&0xf) < 4) {
        add_sub(state, var);
    }
}

void exec_J(mips_cpu_h state, uint32_t var[8]) {
    //TODO
}

void exec_I(mips_cpu_h state, uint32_t var[8]) {
    //TODO
}

void add_sub(mips_cpu_h state, uint32_t var[8]) {
    if((var[FUNC]&0xf) < 2) {
        state->regs[var[REG_D]] = state->regs[var[REG_S]] +
            state->regs[var[REG_T]];
        state->overflow = 0;
        if((var[FUNC]&0xf) == 0) {
            state->overflow = 1;
        }
    }
}
