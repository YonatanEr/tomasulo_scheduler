#ifndef INSTRUCTION_STATE_H
#define INSTRUCTION_STATE_H

#include <stdbool.h>
#include "tag.h"

#define ADD_OPCODE 2
#define SUB_OPCODE 3
#define MULT_OPCODE 4
#define DIV_OPCODE 5
#define HALT_OPCODE 6


typedef struct Inst {
    char opcode;
    char dst;
    char src0;
    char src1;
} Inst;

bool is_halt(int _inst);
Inst parse_inst(int _inst);
void print_inst(Inst inst);


typedef struct InstState {
    Inst inst;
    int pc;
    int cycle_issued;
    Tag res_sta_tag;
    int cycle_execute_start;
    int cycle_execute_end;
    int cycle_write_cdb;
} InstState;

InstState* init_instruction_state(int _inst, int pc);
void free_instruction_state(InstState* inst_state);
void print_inst_state(InstState* inst_state);

#endif