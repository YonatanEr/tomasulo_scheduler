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

float calc_exec_value( CPU* cpu_ptr, InstStateNode* curr_node, int curr_logical_unit_type, int curr_res_sta_idx )
// Assumption: curr_node != NULL
{
    int curr_logical_unit_type, curr_res_sta_idx;
    float vj, vk, exec_val;

    if ( curr_node == NULL )
        assert(NULL);

    vj = cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr[curr_res_sta_idx].vj;
    vk = cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr[curr_res_sta_idx].vk;

    switch (curr_node->inst_state->inst.opcode)
    {
    case ADD_OPCODE:
        exec_val = vj + vk;
        break;
    case SUB_OPCODE:
        exec_val = vj - vk;
        break;
    case MULT_OPCODE:
        exec_val = vj * vk;
        break;
    case DIV_OPCODE:
        exec_val = vj / vk;
        break;    
    default:
        assert(NULL);
        break;
    }
    return exec_val;
}

void write_cdb_update_register_array ( CPU* cpu_ptr, InstStateNode* curr_node, float exec_val )
{
    int dst_reg;
    dst_reg = curr_node->inst_state->inst.dst; // converts char to int

    if ( is_equal_tag ( curr_node->inst_state->res_sta_tag, cpu_ptr->reg_state_arr[dst_reg].q ) )
    {
        cpu_ptr->reg_state_arr[dst_reg].q = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
        cpu_ptr->reg_state_arr[dst_reg].v = exec_val;
    }
}

// Assumption: curr_node != NULL
void write_cdb_update_rs_qjk_when_needed ( CPU* cpu_ptr, InstStateNode* curr_node, int curr_logical_unit_type, int curr_res_sta_idx, float exec_val )
{
    int nr_res_stas, logical_unit_type_idx, res_sta_idx; 
    ResSta* res_sta_arr;

    for ( logical_unit_type_idx = 0; logical_unit_type_idx < LOGICAL_UNIT_TYPES; logical_unit_type_idx++ )
    {
        nr_res_stas = cpu_ptr->logical_unit_arr[curr_logical_unit_type].nr_res_stas;
        res_sta_arr = cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr;
        
        for ( res_sta_idx = 0; res_sta_idx < nr_res_stas; res_sta_idx++ )
        {
            // nothing to update in the current inst.'s res. sta
            if ( curr_logical_unit_type == logical_unit_type_idx && curr_res_sta_idx == res_sta_idx )
                continue; 

            // updating vj if needed 
            if ( is_equal_tag ( res_sta_arr[res_sta_idx].qj, curr_node->inst_state->res_sta_tag ) )
            {
                res_sta_arr[res_sta_idx].vj = exec_val;
                res_sta_arr[res_sta_idx].qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
            }
            // updating vk if needed
            if ( is_equal_tag ( res_sta_arr[res_sta_idx].qk, curr_node->inst_state->res_sta_tag ) )
            {
                res_sta_arr[res_sta_idx].vk = exec_val;
                res_sta_arr[res_sta_idx].qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
            }

        }
    }
}

// shorten the function arguments
void write_cdb_delete_rs_of_curr_int ( CPU* cpu_ptr, int curr_logical_unit_type, int curr_res_sta_idx )
{
    // logical unit update
    cpu_ptr->logical_unit_arr[curr_logical_unit_type].nr_avail_res_stas++;

    // current res sta update:
    // Qj, qk must have been initialized, o.w we wouldn't have executed
    // Tag - A const. value from initialization
    // Vj, Vk - Would be overriden in the next res sta usage 
    cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr[curr_res_sta_idx].busy = false;
}

void write_cdb_update_curr_inst_state ( CPU* cpu_ptr, InstStateNode* curr_node )
{
    curr_node->inst_state->cycle_write_cdb = cpu_ptr->cycle_count;
    curr_node->inst_state->res_sta_tag = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
}
void execute_to_write_cdb ( CPU* cpu_ptr )
{
    InstStateNode* curr_node = cpu_ptr->inst_state_lst;
    bool cdb_used[LOGICAL_UNIT_TYPES];
    int curr_logical_unit_type, curr_res_sta_idx, dst_reg, i;
    float exec_val;

    for ( i = 0; i<LOGICAL_UNIT_TYPES; i++ )
    {
        cdb_used[i] = false;
    }

    while ( curr_node != NULL )
    {
        if ( curr_node->inst_state->cycle_execute_end != NOT_INITIALZIED && cpu_ptr->cycle_count >= curr_node->inst_state->cycle_execute_end + 1 ) 
        { // for all inst. which finished their execution, i.e. curr_cycle >= ex_end_cycle + 1

            curr_logical_unit_type = curr_node->inst_state->res_sta_tag.type;
            curr_res_sta_idx = curr_node->inst_state->res_sta_tag.res_sta_idx;

            if ( !cdb_used[curr_logical_unit_type] ) // cdb was not used already
            {
                // update cdb as used 
                cdb_used[curr_logical_unit_type] = true;

                // calc. the dest reg. value
                exec_val = calc_exec_value( cpu_ptr, curr_node, curr_logical_unit_type, curr_res_sta_idx);

                // updating the register array 
                write_cdb_update_register_array ( cpu_ptr, curr_node, exec_val );

                // updating vj(k), qj(k) of res. sta for which qj(k) = curr inst's tag 
                write_cdb_update_rs_qjk_when_needed ( cpu_ptr, curr_node, curr_logical_unit_type, curr_res_sta_idx, exec_val );

                // deleting the current inst's rs
                write_cdb_delete_rs_of_curr_int ( cpu_ptr, curr_logical_unit_type, curr_res_sta_idx );

                // update the inst. state w/ cycle update. 
                write_cdb_update_curr_inst_state ( cpu_ptr, curr_node );

                // file update: cdb
            }
        }
        curr_node = curr_node->next;
    }
}

void issue_to_execute( CPU* cpu_ptr )
{
    InstStateNode* curr_node = cpu_ptr->inst_state_lst;
    int curr_logical_unit_type, curr_res_sta_idx; 

    while ( curr_node != NULL )
    {
        if ( curr_node->inst_state->cycle_issued != NOT_INITIALZIED && curr_node->inst_state->cycle_execute_start == NOT_INITIALZIED )
        {
            curr_logical_unit_type = curr_node->inst_state->res_sta_tag.type; 
            curr_res_sta_idx = curr_node->inst_state->res_sta_tag.res_sta_idx; 

            if (    cpu_ptr->logical_unit_arr[curr_logical_unit_type].nr_avail_fus > 0  && 
                    cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr[curr_res_sta_idx].qj.type == NOT_INITIALZIED && 
                    cpu_ptr->logical_unit_arr[curr_logical_unit_type].res_sta_arr[curr_res_sta_idx].qk.type == NOT_INITIALZIED )
            {

                curr_node->inst_state->cycle_execute_start = cpu_ptr->cycle_count;
                curr_node->inst_state->cycle_execute_end = cpu_ptr->cycle_count + cpu_ptr->logical_unit_arr[curr_logical_unit_type].fu_delay -1;
                cpu_ptr->logical_unit_arr[curr_logical_unit_type].nr_avail_fus--;
            }
        }

        curr_node = curr_node->next;
    }
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
        execute_to_write_cdb(cpu);
        issue_to_execute(cpu);
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