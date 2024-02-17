#ifndef CPU_H
#define CPU_H

#define REGISTERS_AMOUNT 16
#define LOGICAL_UNIT_TYPES 3

#include <stdbool.h>
#include "register_state.h"
#include "instruction_state.h"
#include "logical_unit.h"

typedef struct CPU {
    RegState reg_state_arr [REGISTERS_AMOUNT];
    InstState* inst_state_lst;
    LogicalUnit logical_unit_arr [LOGICAL_UNIT_TYPES];
    bool halt;
} CPU;


#endif