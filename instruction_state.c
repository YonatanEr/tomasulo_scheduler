#include "instruction_state.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


// returns true iff the instruction is HALT
bool is_halt(int _inst){
    return ((_inst >> 24) & 0xf) == HALT_OPCODE;
}


// return a new struct to represent the parsed instruction
Inst parse_inst(int _inst){
    Inst inst;
    inst.opcode = (_inst >> 24) & 0xf;
    inst.dst    = (_inst >> 20) & 0xf;
    inst.src0   = (_inst >> 16) & 0xf;
    inst.src1   = (_inst >> 12) & 0xf;
    return inst;
}


// maps the given opcode to the index of the logical unit
int opcode2type(Inst inst){
    switch (inst.opcode)
    {
    case ADD_OPCODE:
        return ADD_FU_IDX;
    case SUB_OPCODE:
        return ADD_FU_IDX;
    case MULT_OPCODE:
        return MULT_FU_IDX;
    case DIV_OPCODE:
        return DIV_FU_IDX;
    default:
        assert(NULL);
        break;
    }
}


// prints a single instruction
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
    printf("F[%2d]=F[%2d]%cF[%2d]   ", inst.dst, inst.src0, operation, inst.src1);
}


// returns an initialized pointer to a new instrction state
InstState* init_instruction_state(int _inst, int pc, int cycle_fetched){
    InstState* inst_state = (InstState*) malloc (1 * sizeof(InstState));
    assert(inst_state);
    inst_state->inst = parse_inst(_inst);
    inst_state->pc = pc;
    inst_state->cycle_issued = NOT_INITIALZIED;
    inst_state->res_sta_tag = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    inst_state->cycle_execute_start = NOT_INITIALZIED;
    inst_state->cycle_execute_end = NOT_INITIALZIED;
    inst_state->cycle_write_cdb = NOT_INITIALZIED;
    inst_state->cycle_fetched = cycle_fetched;
    return inst_state;
}


// frees a single instruction state struct
void free_instruction_state(InstState* inst_state){
    free(inst_state);
    inst_state = NULL;
}


// returns true iff the instruction was already issued
bool is_issued(InstState* inst_state){
    return inst_state->cycle_issued != NOT_INITIALZIED;
}


// prints all the instrctions states table
void print_inst_state(InstState* inst_state){
    print_inst(inst_state->inst);
    printf("pc = %2d   ", inst_state->pc);
    printf("cycle_fetched= %2d   ", inst_state->cycle_fetched);
    printf("cycle_issued = %2d   ", inst_state->cycle_issued);
    print_tag(inst_state->res_sta_tag);
    printf("cycle_execute_start = %2d   ", inst_state->cycle_execute_start);
    printf("cycle_execute_end = %2d   ", inst_state->cycle_execute_end);
    printf("cycle_write_cdb= %2d   ", inst_state->cycle_write_cdb);
    printf("\n");
}