#ifndef CPU_H
#define CPU_H
#define REGISTERS_AMOUNT 16

#include <stdbool.h>
#include "registers.h"
#include "adder.h"
#include "multiplier.h"
#include "divider.h"
#include "instruction.h"


typedef struct CPU {
    InstructionQueue instructions_queue;
    REGISTERS* registers;
    Adders* adders;
    Multipliers* multipliers;
    Dividers* dividers;
} CPU;


CPU* init_cpu(char* cfg_file_path);
void free_cpu(CPU* cpu);


#endif