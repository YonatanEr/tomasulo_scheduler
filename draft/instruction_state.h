#ifndef INSTRUCTION_STATE_H
#define INSTRUCTION_STATE_H

#include <stdbool.h>
#include "tag.h"

#define ADD_OPCODE 2
#define SUB_ADD_OPCODE 3
#define MULT_ADD_OPCODE 4
#define DIV_ADD_OPCODE 5
#define HALT_ADD_OPCODE 6


typedef struct Inst {
    char opcode;
    char dst;
    char src0;
    char src1;
} Inst;


typedef struct InstState {
    Inst inst;
    int pc;
    Tag res_sta_tag;
    int cycle_issued;
    int cycle_execute_start;
    int cycle_execute_end;
    int cycle_write_cdb;
    struct InstState* next;
} InstState;

#endif