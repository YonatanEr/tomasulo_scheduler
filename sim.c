#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "sim_args.h"

#define MAX_INSTRUCTION_FILE_LINE_LENGTH 16
#define FETCHES_PER_CYCLE 2
#define ISSUES_PER_CYCLE 2
#define MIN(a,b) ((a) < (b) ? (a) : (b))


void issue_reg_state_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    int dst=inst_state->inst.dst;
    cpu->reg_state_arr[dst].q = res_sta->tag;
}

void issue_res_sta_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    int src0=inst_state->inst.src0;
    if (cpu->reg_state_arr[src0].q.type == NOT_INITIALZIED){
        res_sta->vj = cpu->reg_state_arr[src0].v;
    }
    else{
        res_sta->qj = cpu->reg_state_arr[src0].q;
    }
    int src1=inst_state->inst.src1;
    if (cpu->reg_state_arr[src1].q.type == NOT_INITIALZIED){
        res_sta->vk = cpu->reg_state_arr[src1].v;
    }
    else{
        res_sta->qk = cpu->reg_state_arr[src1].q;
    }
    res_sta->busy = true;
}


void issue_inst_state_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    inst_state->cycle_issued = cpu->cycle_count;
    inst_state->res_sta_tag = res_sta->tag;
}

void issue(CPU* cpu){
    int current_cycle_issues_counter = 0;
    InstStateNode* inst_state_node = cpu->inst_state_lst;
    while (inst_state_node!=NULL && current_cycle_issues_counter<ISSUES_PER_CYCLE){
        LogicalUnit logical_unit = cpu->logical_unit_arr[opcode2type(inst_state_node->inst_state->inst)];
        if (is_issued(inst_state_node->inst_state) || logical_unit.nr_avail_res_stas==0){
            inst_state_node = inst_state_node->next;
            continue;
        }

        // looking for an available rs
        ResSta res_sta = logical_unit.res_sta_arr[get_available_res_sta_idx(&logical_unit)];

        // inst. state update 
        issue_inst_state_update(cpu, inst_state_node->inst_state, &res_sta);

        // rs. update regs update 
        issue_res_sta_update(cpu, inst_state_node->inst_state, &res_sta);

        // registers state update   must come after rs update
        issue_reg_state_update(cpu, inst_state_node->inst_state, &res_sta);

        // logical unit update 
        logical_unit.nr_avail_res_stas--;

        current_cycle_issues_counter++;
        inst_state_node = inst_state_node->next;
    }
    // update trace file
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
    // update trace file
    clean_bypass(cpu);
    clean_head(cpu);
    clean_tail(cpu);
}

void fetch(CPU* cpu, FILE* memin_fp){
    char cycle_fetches = MIN(MAX_INSTRUCTION_STATE_LIST_SIZE - inst_state_lst_len(cpu->inst_state_lst), FETCHES_PER_CYCLE);
    while (cycle_fetches--){
        char* line = (char*) calloc (MAX_INSTRUCTION_FILE_LINE_LENGTH, sizeof(char));
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
    // for each instruction, add cycle fetched
}

void regout(CPU* cpu, char* regout_file_path){
    FILE* fp = fopen(regout_file_path , "w");
    assert(fp);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        fprintf(fp, "%f\n", cpu->reg_state_arr[i].v);
    }
    fclose(fp);
}

void simulate(CPU* cpu, SimArgs sim_args){
    FILE* memin_fp = fopen(sim_args.memin, "r");
    do
    {
        printf("\n\n");
        printf("CYCLE #%d\n", cpu->cycle_count);
        printf("CURRENT INSTRUCTION STATE LIST\n");
        print_inst_state_lst(cpu->inst_state_lst);
        clean(cpu);
        //ex2cdb;
        //issue2cdb;
        issue(cpu);
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
    simulate(cpu, sim_args);
    regout(cpu, sim_args.regout);
    free_cpu(cpu);
}