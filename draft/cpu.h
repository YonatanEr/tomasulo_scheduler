#ifndef CPU_H
#define CPU_H

#define REGISTERS_AMOUNT 16
#define LOGICAL_UNIT_TYPES 3

#include <stdbool.h>
#include "register_state.h"
#include "instruction_state.h"
#include "logical_unit.h"

typedef struct CPU {
    bool halt;
    int cycle_count;
    InstState* inst_state_lst;
    RegState reg_state_arr [REGISTERS_AMOUNT];
    LogicalUnit logical_unit_arr [LOGICAL_UNIT_TYPES];
} CPU;


CPU init_cpu(char* cfg_file_path);
void start_cpu(CPU* cpu);
void free_cpu(CPU* cpu);

#endif