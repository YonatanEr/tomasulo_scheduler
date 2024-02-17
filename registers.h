#ifndef REGISTERS_H
#define REGISTERS_H

#define REGISTERS_AMOUNT 16


typedef struct REGISTERS {
    float F [REGISTERS_AMOUNT];
} REGISTERS;


REGISTERS* init_registers();
void free_registers(REGISTERS* registers);
void print_registers(REGISTERS* registers);
void regout(REGISTERS* registers, char* regout_file_path);

#endif