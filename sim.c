#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "sim_args.h"

#define MAX_INSTRUCTION_FILE_LINE_LENGTH 16
#define FETCH_PER_CYCLE 2
#define MIN(a,b) ((a) < (b) ? (a) : (b))


void regout(CPU* cpu, char* regout_file_path){
    FILE* fp = fopen(regout_file_path , "w");
    assert(fp);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        fprintf(fp, "%f\n", cpu->reg_state_arr[i].v);
    }
    fclose(fp);
}

void fetch(CPU* cpu, FILE* memin_fp){
    char* line;
    char cycle_fetches = MIN(MAX_INSTRUCTION_STATE_LIST_SIZE - inst_state_lst_len(cpu->inst_state_lst), FETCH_PER_CYCLE);
    while (cycle_fetches--){
        printf("\n\nCURRENT INSTRUCTION STATE LIST\n");
        print_inst_state_lst(cpu->inst_state_lst);
        line = (char*) calloc (MAX_INSTRUCTION_FILE_LINE_LENGTH, sizeof(char));
        assert(line);
        fgets(line, MAX_INSTRUCTION_FILE_LINE_LENGTH, memin_fp);
        int _inst = strtol(line, NULL, 16);
        free(line);
        line = NULL;
        if (is_halt(_inst)){
            cpu->halt = true;
            break;
        }
        else{
            insert_inst_state(&(cpu->inst_state_lst), _inst, cpu->pc++);
        }
    }
}

bool can_be_cleaned(CPU* cpu, InstStateNode* node){
    return (node->inst_state->cycle_write_cdb != NOT_INITIALZIED) && (node->inst_state->cycle_write_cdb < cpu->cycle_count);
}

void clean_head(CPU* cpu){
    if (cpu->inst_state_lst == NULL){
        return;
    }
    InstStateNode* cur_head;
    if (can_be_cleaned(cpu, cpu->inst_state_lst))
    {
        cur_head = cpu->inst_state_lst;
        cpu->inst_state_lst = cur_head->next;
        free_inst_state_node(cur_head);
    }
}

void clean_bypass(CPU* cpu){
    if (cpu->inst_state_lst == NULL || cpu->inst_state_lst->next == NULL || cpu->inst_state_lst->next->next == NULL){
        return;
    }
    InstStateNode* prev_node = cpu->inst_state_lst;
    InstStateNode* cur_node = prev_node->next;
    InstStateNode* next_node = cur_node->next;
    while (next_node != NULL){
        if (can_be_cleaned(cpu, cur_node)){
            free_inst_state_node(cur_node);
            prev_node->next = next_node;
        }
        else{
            prev_node = cur_node;
        }
        cur_node = next_node;
        next_node = next_node->next;
    }
}

void clean_tail(CPU* cpu){
    if (inst_state_lst_len(cpu->inst_state_lst) < 2){
        return;
    }
    InstStateNode* prev_node = cpu->inst_state_lst;
    InstStateNode* cur_node = prev_node->next;
    while (cur_node->next != NULL)
    {
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    if (can_be_cleaned(cpu, cur_node)){
        free_inst_state_node(cur_node);
        prev_node->next = NULL;
    }
}

void clean(CPU* cpu){
    clean_bypass(cpu);
    clean_head(cpu);
    clean_tail(cpu);
}


void start_simulation(CPU* cpu, SimArgs sim_args){
    FILE* memin_fp = fopen(sim_args.memin, "r");
    do
    {
        //ex2cdb;
        //issue2cdb;
        //issue();
        clean(cpu);
        if (!cpu->halt){
            fetch(cpu, memin_fp);
        }
        cpu->cycle_count++;
    } while (cpu->inst_state_lst);
    fclose(memin_fp);
}

int main(int argc, char **argv){
    SimArgs sim_args = parse_args(argc, argv);
    CPU* cpu = init_cpu(sim_args.cfg);
    start_simulation(cpu, sim_args);
    regout(cpu, sim_args.regout);
    free_cpu(cpu);
}