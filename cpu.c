#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CFG_FILE_LINE_LENGTH 64

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

int parameter(char* line){
    char* token = NULL;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    return atoi(token);
}

void read_cpu_cfg(CPU* cpu, char* cfg_file_path){
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

CPU* init_cpu(char* cfg_file_path){
    CPU* cpu = (CPU*) calloc (1, sizeof(CPU));
    assert(cpu);
    read_cpu_cfg(cpu, cfg_file_path);
    cpu->halt = false;
    cpu->cycle_count = 0;
    cpu->pc = 0;
    cpu->inst_state_lst = NULL;
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        cpu->reg_state_arr[i].v = i;
        cpu->reg_state_arr[i].q = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    }
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        LogicalUnit* logical_unit = cpu->logical_unit_arr[type];
        init_logical_unit(&logical_unit, type);
        printf("logical_unit.nr_res_stas = %d\n", logical_unit->nr_res_stas);
        for (int index=0; index < logical_unit->nr_res_stas; index++){
            printf("res_sta_arr[index].busy = %d\n", logical_unit->res_sta_arr[index].busy);
    }

    }
    return cpu;
}

void free_cpu(CPU* cpu){
    free_inst_state_lst(cpu->inst_state_lst);
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        LogicalUnit* logical_unit = cpu->logical_unit_arr[type];
        free_logical_unit(logical_unit);
    }
    free(cpu);
    cpu = NULL;
}

