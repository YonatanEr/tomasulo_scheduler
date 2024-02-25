#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CFG_FILE_LINE_LENGTH 16

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

void read_cpu_cfg(CPU cpu, char* cfg_file_path){
    FILE* fp = fopen(cfg_file_path, "r");
    assert(fp);
    char line [MAX_CFG_FILE_LINE_LENGTH];
    while (fgets(line, MAX_CFG_FILE_LINE_LENGTH, fp) != NULL){
        if (startswith("add_nr_units", line)){
            cpu.logical_unit_arr[ADD_FU_IDX].nr_fus = parameter(line);
            continue;
        }
        if (startswith("mul_nr_units", line)){
            cpu.logical_unit_arr[MULT_FU_IDX].nr_fus = parameter(line);
            continue;
        }
        if (startswith("div_nr_units", line)){
            cpu.logical_unit_arr[DIV_FU_IDX].nr_fus = parameter(line);
            continue;
        }
        if (startswith("add_nr_reservation", line)){
            cpu.logical_unit_arr[ADD_FU_IDX].nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("mul_nr_reservation", line)){
            cpu.logical_unit_arr[MULT_FU_IDX].nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("div_nr_reservation", line)){
            cpu.logical_unit_arr[DIV_FU_IDX].nr_res_stas = parameter(line);
            continue;
        }
        if (startswith("add_delay", line)){
            cpu.logical_unit_arr[ADD_FU_IDX].fu_delay = parameter(line);
            continue;
        }
        if (startswith("mul_delay", line)){
            cpu.logical_unit_arr[MULT_FU_IDX].fu_delay = parameter(line);
            continue;
        }
        if (startswith("div_delay", line)){
            cpu.logical_unit_arr[DIV_FU_IDX].fu_delay = parameter(line);
            continue;
        }
    }
    fclose(fp);
}

void init_cpu_logical_units(CPU cpu, char* cfg_file_path){
    for (int i=0; i<LOGICAL_UNIT_TYPES; i++){
        init_logical_unit(cpu.logical_unit_arr[i]);
    }
}

CPU init_cpu(char* cfg_file_path){
    CPU cpu;
    cpu.halt = false;
    cpu.cycle_count = 0;
    cpu.inst_state_lst = NULL;
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        cpu.reg_state_arr[i].v = i;
    }
    init_cpu_logical_units(cpu, cfg_file_path);
    return cpu;
}

void start_cpu(CPU* cpu){
    do
    {
        ex2cdb;
        issue2cdb;
        issue();
        clean();
        if (!cpu->halt){
            fetch();
        }
        (cpu->cycle_count)++;
    } while (cpu->inst_state_lst != NULL);
}

void free_inst_state_lst(InstState* inst_state){
    if (inst_state != NULL){
        free_inst_state_lst(inst_state->next);
        free(inst_state);
        inst_state = NULL;
    }
}

void free_cpu(CPU* cpu){
    free_inst_state_lst(cpu->inst_state_lst);
    free(cpu);
    cpu = NULL;
}
