#ifndef INSTRUCTION_STATE_TRACE_H
#define INSTRUCTION_STATE_TRACE_H

#include "instruction_state.h"

typedef struct InstStateTrace {
    InstState inst_state;
    struct InstStateTrace* next;
} InstStateTrace;


void free_inst_state_trace(InstStateTrace* node);
void insert_to_inst_state_trace(InstStateTrace** lst, InstState inst_state);


#endif