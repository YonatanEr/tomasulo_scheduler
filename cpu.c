#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CFG_FILE_LINE_LENGTH 64

// returns true iff str starts with prefix
bool startswith(char* prefix, char* str){
    int prefix_len=strlen(prefix), str_len=strlen(str);
    if (prefix_len > str_len){
        return false;
    }
    for (int i=0; i<prefix_len; i++){
        if (prefix[i] != str[i]){
            return false;
        }
    }
    return true;
}

// parses the parameter from a line
int parameter(char* line){
    char* token = NULL;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    return atoi(token);
}

// reads the cpu cfg file to update in the cpu struct
void read_cpu_cfg(CPU** cpu_ptr, char* cfg_file_path){
    CPU* cpu = *cpu_ptr;
    FILE* fp = fopen(cfg_file_path, "r");
    assert(fp);
    char line [MAX_CFG_FILE_LINE_LENGTH];
    while (fgets(line, MAX_CFG_FILE_LINE_LENGTH, fp) != NULL){
        if (startswith("add_nr_units", line)){
            cpu->logical_unit_arr[ADD_FU_IDX]->nr_fus = parameter(line);
            continue;
        }
        if (startswith("mul_nr_units", line)){
            cpu->logical_unit_arr[MULT_FU_IDX]->nr_fus = parameter(line);
            continue;
        }
        if (startswith("div_nr_units", line)){
            cpu->logical_unit_arr[DIV_FU_IDX]->nr_fus = parameter(line);
            continue;
        }
        if (startswith("add_nr_reservation", line)){
            cpu->logical_unit_arr[ADD_FU_IDX]->nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("mul_nr_reservation", line)){
            cpu->logical_unit_arr[MULT_FU_IDX]->nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("div_nr_reservation", line)){
            cpu->logical_unit_arr[DIV_FU_IDX]->nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("add_delay", line)){
            cpu->logical_unit_arr[ADD_FU_IDX]->fu_delay = parameter(line);
            continue;
        }
        if (startswith("mul_delay", line)){
            cpu->logical_unit_arr[MULT_FU_IDX]->fu_delay = parameter(line);
            continue;
        }
        if (startswith("div_delay", line)){
            cpu->logical_unit_arr[DIV_FU_IDX]->fu_delay = parameter(line);
            continue;
        }
    }
    fclose(fp);
}

// initialize a new instance of CPU with all attributes initilized
CPU* init_cpu(char* cfg_file_path){
    CPU* cpu = (CPU*) calloc (1, sizeof(CPU));
    assert(cpu);
    cpu->halt = false;
    cpu->cycle = 0;
    cpu->pc = 0;
    cpu->inst_state_lst = NULL;
    cpu->inst_state_trace = NULL;
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        cpu->reg_state_arr[i].v = i;
        cpu->reg_state_arr[i].q = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    }
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        cpu->logical_unit_arr[type] = (LogicalUnit*) calloc (1, sizeof(LogicalUnit));
        assert(cpu->logical_unit_arr[type]);
    }
    read_cpu_cfg(&cpu, cfg_file_path);
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        LogicalUnit* logical_unit = cpu->logical_unit_arr[type];
        init_logical_unit(&logical_unit, type);
    }
    return cpu;
}

// frees the whole CPU struct
void free_cpu(CPU* cpu){
    free_inst_state_lst(cpu->inst_state_lst);
    free_inst_state_trace(cpu->inst_state_trace);
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        LogicalUnit* logical_unit = cpu->logical_unit_arr[type];
        free_logical_unit(logical_unit);
    }
    free(cpu);
    cpu = NULL;
}