#include "registers.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


REGISTERS* init_registers(){
    REGISTERS* registers = (REGISTERS*) calloc (1, sizeof(REGISTERS));
    assert(registers);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        registers->F[i] = i;
    }
    return registers;
}


void free_registers(REGISTERS* registers){
    free(registers);
    registers = NULL;
}


void regout(REGISTERS* registers, char* regout_file_path){
    FILE* fp = fopen(regout_file_path , "w");
    assert(fp);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        fprintf(fp, "%f\n", registers->F[i]);
    }
    fclose(fp);
}


void print_registers(REGISTERS* registers){
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        printf("F[%d] = %f\n", i, registers->F[i]);
    }
}