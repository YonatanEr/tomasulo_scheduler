#include "cpu.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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


void set_cpu_cfg(CPU* cpu, char* cfg_file_path){
    FILE* fp = fopen(cfg_file_path, "r");
    assert(fp);
    char* line = (char*) calloc (1, MAX_CFG_FILE_LINE_LENGTH);
    assert(line);
    while (fgets(line, MAX_CFG_FILE_LINE_LENGTH, fp) != NULL){
        if (startswith("add_nr_units", line)){
            cpu->adders->add_nr_units = parameter(line);
            continue;
        }
        if (startswith("mul_nr_units", line)){
            cpu->multipliers->mul_nr_units = parameter(line);
            continue;
        }
        if (startswith("div_nr_units", line)){
            cpu->dividers->div_nr_units = parameter(line);
            continue;
        }
        if (startswith("add_nr_reservation", line)){
            cpu->adders->add_nr_reservation = parameter(line);
            continue;
        }
        if (startswith("mul_nr_reservation", line)){
            cpu->multipliers->mul_nr_reservation = parameter(line);
            continue;
        }
        if (startswith("div_nr_reservation", line)){
            cpu->dividers->div_nr_reservation = parameter(line);
            continue;
        }
        if (startswith("add_delay", line)){
            cpu->adders->add_delay = parameter(line);
            continue;
        }
        if (startswith("mul_delay", line)){
            cpu->multipliers->mul_delay = parameter(line);
            continue;
        }
        if (startswith("div_delay", line)){
            cpu->dividers->div_delay = parameter(line);
            continue;
        }
    }
    fclose(fp);
    free(line);
    line = NULL;
    set_up_adder_units(cpu->adders);
    set_up_multiplier_units(cpu->multipliers);
    set_up_divider_units(cpu->dividers);
}


CPU* init_cpu(char* cfg_file_path){
    CPU* cpu = (CPU*) calloc (1, sizeof(CPU));
    assert(cpu);
    empty_instruction_queue(cpu->instructions_queue);
    cpu->registers = init_registers();
    cpu->adders = init_adders();
    cpu->multipliers = init_multipliers();
    cpu->dividers = init_dividers();
    set_cpu_cfg(cpu, cfg_file_path);
    return cpu;
}


void free_cpu(CPU* cpu){
    free_registers(cpu->registers);
    free_adders(cpu->adders);
    free_multipliers(cpu->multipliers);
    free_dividers(cpu->dividers);
    free(cpu);
    cpu = NULL;
}