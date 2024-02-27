#include "instruction_state.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


bool is_halt(int _inst){
    return ((_inst >> 24) & 0xf) == HALT_OPCODE;
}

Inst parse_inst(int _inst){
    Inst inst;
    inst.opcode = (_inst >> 24) & 0xf;
    inst.dst    = (_inst >> 20) & 0xf;
    inst.src0   = (_inst >> 16) & 0xf;
    inst.src1   = (_inst >> 12) & 0xf;
    return inst;
}

void print_inst(Inst inst){
    char operation;
    switch (inst.opcode)
    {
    case ADD_OPCODE:
        operation = '+';
        break;
    case SUB_OPCODE:
        operation = '-';
        break;
    case MULT_OPCODE:
        operation = '*';
        break;
    case DIV_OPCODE:
        operation = '/';
        break;    
    default:
        assert(NULL);
        break;
    }
    printf("F[%d]=F[%d]%cF[%d]\n", inst.dst, inst.src0, operation, inst.src1);
}


InstState* init_instruction_state(int _inst, int pc){
    InstState* inst_state = (InstState*) malloc (1 * sizeof(InstState));
    assert(inst_state);
    inst_state->inst = parse_inst(_inst);
    inst_state->pc = pc;
    inst_state->cycle_issued = -1;
    inst_state->res_sta_tag.type = -1;
    inst_state->res_sta_tag.res_sta_idx = -1;
    inst_state->cycle_execute_start = -1;
    inst_state->cycle_execute_end = -1;
    inst_state->cycle_write_cdb = -1;
    return inst_state;
}

void free_instruction_state(InstState* inst_state){
    free(inst_state);
    inst_state = NULL;
}

void print_inst_state(InstState* inst_state){
    print_inst(inst_state->inst);
    printf("pc = %d\n", inst_state->pc);
    printf("cycle_issued = %d\n", inst_state->cycle_issued);
    print_tag(inst_state->res_sta_tag);
    printf("cycle_execute_start = %d\n", inst_state->cycle_execute_start);
    printf("cycle_execute_end = %d\n", inst_state->cycle_execute_end);
    printf("cycle_write_cdb = %d\n", inst_state->cycle_write_cdb);
}