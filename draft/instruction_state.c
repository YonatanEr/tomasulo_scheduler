#include "instruction_state.h"
#include <assert.h>
#include <stdlib.h>


bool is_halt(int _inst){
    return ((_inst >> 24) & 0xf) == HALT_ADD_OPCODE;
}

Inst parse_inst(int _inst){
    Inst inst;
    inst.opcode = (_inst >> 24) & 0xf;
    inst.dst    = (_inst >> 20) & 0xf;
    inst.src0   = (_inst >> 16) & 0xf;
    inst.src1   = (_inst >> 12) & 0xf;
    return inst;
}

InstState* init_instruction_state(int _inst, int pc){
    InstState* inst_state = (InstState*) calloc (1, sizeof(InstState));
    assert(inst_state);
    inst_state->inst = parse_inst(_inst);
    inst_state->pc = pc;
    return inst_state;
}

void free_instruction_state(InstState* inst_state){
    free(inst_state);
    inst_state = NULL;
}