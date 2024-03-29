#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "sim_args.h"

#define MAX_INSTRUCTION_FILE_LINE_LENGTH 16
#define FETCHES_PER_CYCLE 2
#define ISSUES_PER_CYCLE 2
#define MIN(a,b) ((a) < (b) ? (a) : (b))


// prints the current instructions state status
void print_instructions_state_status(InstStateNode* inst_state_node){
    printf("\nCURRENT INSTRUCTIONS STATE STATUS\n");
    print_inst_state_lst(inst_state_node);    
}


// prints the current registers status
void print_registers_status(RegState reg_state_arr [REGISTERS_AMOUNT]){
    printf("\nCURRENT REGISTERS STATE STATUS\n");
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        printf("        ----------------------------------------------------------\n");
        printf("        ");
        printf("F[%2d]   ", i);
        printf("value = %10f   ", reg_state_arr[i].v);
        print_tag(reg_state_arr[i].q);
        printf("\n");
    }
    printf("        ----------------------------------------------------------\n");
}


// prints the current logical units
void print_logical_units_status(LogicalUnit* logical_unit_arr [LOGICAL_UNIT_TYPES]){
    printf("\nCURRENT LOGICAL UNITS STATUS\n");
    char* idx2name[3] = {"ADD", "MUL", "DIV"};
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        printf("    %s:\n", idx2name[type]);
        print_logical_unit(logical_unit_arr[type]);
    }
}


// prints the current simulation status
void print_status(CPU* cpu){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CYCLE #%d ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", cpu->cycle);
    printf("cpu->halt = %s\n\n", cpu->halt?"true":"false");
    print_instructions_state_status(cpu->inst_state_lst);
    print_registers_status(cpu->reg_state_arr);
    print_logical_units_status(cpu->logical_unit_arr);
}


// updates the register state which was just issued
void issue_reg_state_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    int dst = inst_state->inst.dst;
    cpu->reg_state_arr[dst].q = res_sta->tag;
    for (int type=0; type<LOGICAL_UNIT_TYPES; type++){
        if (cpu->cdb_state_arr[type].dst_reg == dst){
            cpu->cdb_state_arr[type].update_reg_file = false;
        }
    }
}


// returns the index which is waiting for the dst reg, if there isn't -1
int check_cdb_value_to_res_sta_copy(CPU* cpu, int src_reg )
{
    int ret_val = NOT_INITIALZIED; 

    for ( int i=0; i<LOGICAL_UNIT_TYPES; i++)
    {
        if ( src_reg == cpu->cdb_state_arr[i].dst_reg )
        {
            ret_val = i; 
        }
    }

    return ret_val;
}


// updates the reservation station of the issued instructions
void issue_res_sta_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    int cdb_idx=NOT_INITIALZIED, src0=inst_state->inst.src0, src1=inst_state->inst.src1;
    if (cpu->reg_state_arr[src0].q.type == NOT_INITIALZIED){
        res_sta->vj = cpu->reg_state_arr[src0].v;
    }
    else
    {
        cdb_idx = check_cdb_value_to_res_sta_copy( cpu, src0 );
        if ( cdb_idx != NOT_INITIALZIED ) 
            res_sta->vj = cpu->cdb_state_arr[cdb_idx].cdb_value;
        else 
            res_sta->qj = cpu->reg_state_arr[src0].q;
    }
    if (cpu->reg_state_arr[src1].q.type == NOT_INITIALZIED){
        res_sta->vk = cpu->reg_state_arr[src1].v;
    }
    else
    {
        cdb_idx = check_cdb_value_to_res_sta_copy( cpu, src1 );
        if ( cdb_idx != NOT_INITIALZIED ) 
            res_sta->vk = cpu->cdb_state_arr[cdb_idx].cdb_value;
        else
            res_sta->qk = cpu->reg_state_arr[src1].q;
    }
    res_sta->busy = true;
}


// updates the reservation station of the issued instructions
void issue_inst_state_update(CPU* cpu, InstState* inst_state, ResSta* res_sta){
    inst_state->cycle_issued = cpu->cycle;
    inst_state->res_sta_tag = res_sta->tag;
}


// issues up to 2 new instrctions 
void issue(CPU* cpu){
    int current_cycle_issues_counter = 0;
    InstStateNode* inst_state_node = cpu->inst_state_lst;
    while (inst_state_node!=NULL && current_cycle_issues_counter<ISSUES_PER_CYCLE){
        LogicalUnit* logical_unit = cpu->logical_unit_arr[opcode2type(inst_state_node->inst_state->inst)];
        if (is_issued(inst_state_node->inst_state)){
            inst_state_node = inst_state_node->next;
            continue;
        }
        if (logical_unit->nr_avail_res_stas==0){
            break;
        }
        ResSta* res_sta = &(logical_unit->res_sta_arr[get_available_res_sta_idx(logical_unit)]);
        issue_inst_state_update(cpu, inst_state_node->inst_state, res_sta);
        issue_res_sta_update(cpu, inst_state_node->inst_state, res_sta);
        issue_reg_state_update(cpu, inst_state_node->inst_state, res_sta);
        logical_unit->nr_avail_res_stas--;
        current_cycle_issues_counter++;
        inst_state_node = inst_state_node->next;
    }
    // update trace file
}


// returns true iff the instruction is terminated
bool can_be_cleaned(CPU* cpu, InstStateNode* node){
    return (node->inst_state->cycle_write_cdb != NOT_INITIALZIED) && (node->inst_state->cycle_write_cdb < cpu->cycle);
}


// cleans the beginning instruction if terminated
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


// cleans all middle instructions which are terminated
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


// cleans the last instruction if terminated
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


// cleans terminated instructions
void clean(CPU* cpu){
    clean_bypass(cpu);
    clean_head(cpu);
    clean_tail(cpu);
}


// fetches up to 2 new instructions
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
            insert_inst_state(&(cpu->inst_state_lst), _inst, cpu->pc++, cpu->cycle);
        }
    }
}


// computes the values for all the available functional units 
float calc_exec_value( CPU* cpu_ptr, InstStateNode* curr_node, int curr_logical_unit_type, int curr_res_sta_idx )
{
    float vj, vk, exec_val;

    if ( curr_node == NULL )
        assert(NULL);

    vj = cpu_ptr->logical_unit_arr[curr_logical_unit_type]->res_sta_arr[curr_res_sta_idx].vj;
    vk = cpu_ptr->logical_unit_arr[curr_logical_unit_type]->res_sta_arr[curr_res_sta_idx].vk;

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


// after updating the cdb, updates the registers values
void write_cdb_update_register_array ( CPU* cpu_ptr )
{
    int dst_reg;
    for ( int i=0; i<LOGICAL_UNIT_TYPES; i++ )
    { 
        dst_reg = cpu_ptr->cdb_state_arr[i].dst_reg;
        if ( cpu_ptr->cdb_state_arr[i].update_reg_file )
        {
            cpu_ptr->reg_state_arr[dst_reg].v = cpu_ptr->cdb_state_arr[i].cdb_value;
            cpu_ptr->reg_state_arr[dst_reg].q = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
        }
    }
}


// with a used cdb checks if there is some reservation station waiting for a value
void write_cdb_update_rs_qjk_when_needed ( CPU* cpu_ptr, CdbState* curr_cdb_state )
{
    int nr_res_stas, logical_unit_type_idx, res_sta_idx; 
    ResSta* res_sta_arr;

    for ( logical_unit_type_idx = 0; logical_unit_type_idx < LOGICAL_UNIT_TYPES; logical_unit_type_idx++ )
    {
        nr_res_stas = cpu_ptr->logical_unit_arr[logical_unit_type_idx]->nr_res_stas;
        res_sta_arr = cpu_ptr->logical_unit_arr[logical_unit_type_idx]->res_sta_arr;
        
        for ( res_sta_idx = 0; res_sta_idx < nr_res_stas; res_sta_idx++ )
        {
            // nothing to update in the current inst.'s res. sta
            if ( curr_cdb_state->res_sta_tag.type == logical_unit_type_idx && curr_cdb_state->res_sta_tag.res_sta_idx == res_sta_idx )
                continue; 

            // updating vj if needed 
            if ( is_equal_tag ( res_sta_arr[res_sta_idx].qj, curr_cdb_state->res_sta_tag ) )
            {
                res_sta_arr[res_sta_idx].vj = curr_cdb_state->cdb_value;
                res_sta_arr[res_sta_idx].qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
            }
            // updating vk if needed
            if ( is_equal_tag ( res_sta_arr[res_sta_idx].qk, curr_cdb_state->res_sta_tag ) )
            {
                res_sta_arr[res_sta_idx].vk = curr_cdb_state->cdb_value;
                res_sta_arr[res_sta_idx].qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
            }

        }
    }
}


// iterates over the cdb and checks if there is some reservation station waiting for a value
void wrapper_write_cdb_update_rs_qjk_when_needed ( CPU* cpu_ptr )
{
    for ( int i=0; i<LOGICAL_UNIT_TYPES; i++ )
    {
        if ( cpu_ptr->cdb_state_arr[i].cdb_used )
            write_cdb_update_rs_qjk_when_needed ( cpu_ptr, &cpu_ptr->cdb_state_arr[i] );
    }
}


// updates the write cdb attribute in the instruction state
void write_cdb_update_curr_inst_state ( CPU* cpu_ptr, InstStateNode* curr_node )
{
    curr_node->inst_state->cycle_write_cdb = cpu_ptr->cycle;
}


// clears the current cdb attributes
void initialize_cdb_state ( CPU* cpu_ptr )
{
    for ( int i = 0; i<LOGICAL_UNIT_TYPES; i++ )
    {
        cpu_ptr->cdb_state_arr[i].cdb_used = false;
        cpu_ptr->cdb_state_arr[i].cdb_value = NOT_INITIALZIED; 
        cpu_ptr->cdb_state_arr[i].dst_reg = NOT_INITIALZIED;
        cpu_ptr->cdb_state_arr[i].res_sta_tag = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
        cpu_ptr->cdb_state_arr[i].update_reg_file = false;
    }
}


// updates all the instrctions that are in execute status, and waiting for write cdb
void execute_to_write_cdb ( CPU* cpu_ptr , char* tracecdb)
{
    char* idx2name[3] = {"ADD", "MUL", "DIV"};
    InstStateNode* curr_node = cpu_ptr->inst_state_lst;
    int curr_logical_unit_type, curr_res_sta_idx;
    float exec_val;
    int dst_reg; 

    initialize_cdb_state ( cpu_ptr ); 

    while ( curr_node != NULL )
    {
        if ( curr_node->inst_state->cycle_execute_end != NOT_INITIALZIED && cpu_ptr->cycle >= curr_node->inst_state->cycle_execute_end + 1 ) 
        { // for all inst. which finished their execution, i.e. curr_cycle >= ex_end_cycle + 1

            curr_logical_unit_type = curr_node->inst_state->res_sta_tag.type;
            curr_res_sta_idx = curr_node->inst_state->res_sta_tag.res_sta_idx;

            // Only if the current cycle == cycle_exec_end + 1 ==> release the relevant fu, it happens before the issue.
            if ( cpu_ptr->cycle == curr_node->inst_state->cycle_execute_end + 1 ) 
                cpu_ptr->logical_unit_arr[curr_logical_unit_type]->nr_avail_fus++;

            if ( !cpu_ptr->cdb_state_arr[curr_logical_unit_type].cdb_used ) // cdb was not used already
            {
                // update cdb as used 
                cpu_ptr->cdb_state_arr[curr_logical_unit_type].cdb_used = true;

                // calc. the dest reg. value
                exec_val = calc_exec_value( cpu_ptr, curr_node, curr_logical_unit_type, curr_res_sta_idx);
                cpu_ptr->cdb_state_arr[curr_logical_unit_type].cdb_value = exec_val;

                // Store the dst_reg. and the tag of the current node for the comparison w/ the Tags of the res. sta and reg. state
                cpu_ptr->cdb_state_arr[curr_logical_unit_type].res_sta_tag = curr_node->inst_state->res_sta_tag; 
                dst_reg = curr_node->inst_state->inst.dst; // converts char to int

                // Prep. for reg. state update in the next cycle start - Only if needed 
                if ( is_equal_tag ( curr_node->inst_state->res_sta_tag, cpu_ptr->reg_state_arr[dst_reg].q ) )
                {
                    cpu_ptr->cdb_state_arr[curr_logical_unit_type].dst_reg = curr_node->inst_state->inst.dst;
                    cpu_ptr->cdb_state_arr[curr_logical_unit_type].update_reg_file = true;
                }

                // updating vj(k), qj(k) of res. sta for which qj(k) = curr inst's tag ==> happens only after ececution, i.e. on the next inst. start

                // deleting the current inst's rs happens only after issue

                // update the inst. state w/ cycle update. 
                write_cdb_update_curr_inst_state ( cpu_ptr, curr_node );

                // tracecdb update
                FILE* fp = fopen(tracecdb , "a");
                assert(fp);
                fprintf(fp, "%d ", curr_node->inst_state->cycle_write_cdb);
                fprintf(fp, "%d ", curr_node->inst_state->pc);
                fprintf(fp, "%s ", idx2name[curr_node->inst_state->res_sta_tag.type]);
                fprintf(fp, "%f ", cpu_ptr->cdb_state_arr[curr_logical_unit_type].cdb_value);
                fprintf(fp, "%s%d\n", idx2name[curr_node->inst_state->res_sta_tag.type], curr_node->inst_state->res_sta_tag.res_sta_idx);
                fclose(fp);

                // traceinst insert
                insert_to_inst_state_trace(&(cpu_ptr->inst_state_trace), *(curr_node->inst_state));
            }
        }
        curr_node = curr_node->next;
    }

}


// after using th cdb, clears the data in the reservation station
void write_cdb_delete_rs ( CPU* cpu_ptr )
{
    InstStateNode* curr_node = cpu_ptr->inst_state_lst;
    int curr_logical_unit_type, curr_res_sta_idx;
    while ( curr_node != NULL )
    {
        if ( curr_node->inst_state->cycle_write_cdb != NOT_INITIALZIED )
        {
            curr_logical_unit_type = curr_node->inst_state->res_sta_tag.type;
            curr_res_sta_idx = curr_node->inst_state->res_sta_tag.res_sta_idx;

            // logical unit update
            cpu_ptr->logical_unit_arr[curr_logical_unit_type]->nr_avail_res_stas++;

            // current res sta update:
            // Qj, qk must have been initialized, o.w we wouldn't have executed
            // Tag - A const. value from initialization
            // Vj, Vk - Would be overriden in the next res sta usage 
            cpu_ptr->logical_unit_arr[curr_logical_unit_type]->res_sta_arr[curr_res_sta_idx].busy = false;

            // Inst. state update - The res. station has been released:
            curr_node->inst_state->res_sta_tag = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
        }

        curr_node = curr_node->next;
    }
}


// updates all the instrctions that are in issue status, and can start execution
void issue_to_execute_start( CPU* cpu_ptr )
{
    InstStateNode* curr_node = cpu_ptr->inst_state_lst;
    int curr_logical_unit_type, curr_res_sta_idx; 

    while ( curr_node != NULL )
    {
        if ( curr_node->inst_state->cycle_issued != NOT_INITIALZIED && curr_node->inst_state->cycle_execute_start == NOT_INITIALZIED )
        {
            curr_logical_unit_type = curr_node->inst_state->res_sta_tag.type; 
            curr_res_sta_idx = curr_node->inst_state->res_sta_tag.res_sta_idx; 

            if (    cpu_ptr->logical_unit_arr[curr_logical_unit_type]->nr_avail_fus > 0  && 
                    cpu_ptr->logical_unit_arr[curr_logical_unit_type]->res_sta_arr[curr_res_sta_idx].qj.type == NOT_INITIALZIED && 
                    cpu_ptr->logical_unit_arr[curr_logical_unit_type]->res_sta_arr[curr_res_sta_idx].qk.type == NOT_INITIALZIED )
            {

                curr_node->inst_state->cycle_execute_start = cpu_ptr->cycle;
                curr_node->inst_state->cycle_execute_end = cpu_ptr->cycle + cpu_ptr->logical_unit_arr[curr_logical_unit_type]->fu_delay -1;
                cpu_ptr->logical_unit_arr[curr_logical_unit_type]->nr_avail_fus--;
            }
        }

        curr_node = curr_node->next;
    }
}


// runs the simulation till no pending instructions
void simulate(CPU* cpu, SimArgs sim_args)
{
    FILE* memin_fp = fopen(sim_args.memin, "r");
    do
    {
        wrapper_write_cdb_update_rs_qjk_when_needed(cpu);
        write_cdb_update_register_array (cpu);
        write_cdb_delete_rs(cpu); // deleting the res. stations from prev. round. Immediately after that they would be cleaned
        clean(cpu);
        execute_to_write_cdb(cpu, sim_args.tracecdb);
        issue_to_execute_start(cpu);
        issue(cpu);
        if (!cpu->halt)
        {
            fetch(cpu, memin_fp);
        }
        cpu->cycle++;
    } while (cpu->inst_state_lst);
    fclose(memin_fp);
}


// writes the regout file
void regout(CPU* cpu, char* regout_file_path){
    FILE* fp = fopen(regout_file_path , "w");
    assert(fp);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        fprintf(fp, "%f\n", cpu->reg_state_arr[i].v);
    }
    fclose(fp);
}


// writes the traceinst file
void traceinst(CPU* cpu, char* traceinst_file_path){
    FILE* fp = fopen(traceinst_file_path , "w");
    assert(fp);
    InstStateTrace* node = cpu->inst_state_trace;
    char* idx2name[3] = {"ADD", "MUL", "DIV"};
    while (node != NULL){
        fprintf(fp, "0%x%x%x%x000 ", node->inst_state.inst.opcode, node->inst_state.inst.dst, node->inst_state.inst.src0, node->inst_state.inst.src1);
        fprintf(fp, "%d ", node->inst_state.pc);
        fprintf(fp, "%s%d ", idx2name[node->inst_state.res_sta_tag.type], node->inst_state.res_sta_tag.res_sta_idx);
        fprintf(fp, "%d ", node->inst_state.cycle_issued);
        fprintf(fp, "%d ", node->inst_state.cycle_execute_start);
        fprintf(fp, "%d ", node->inst_state.cycle_execute_end);
        fprintf(fp, "%d\n", node->inst_state.cycle_write_cdb);
        node = node->next;
    }
    fclose(fp);
}


// runs the whole scenario
int main(int argc, char **argv){
    SimArgs sim_args = parse_args(argc, argv);
    FILE* fp = fopen(sim_args.tracecdb , "w");
    assert(fp);
    fclose(fp);
    CPU* cpu = init_cpu(sim_args.cfg);
    simulate(cpu, sim_args);
    regout(cpu, sim_args.regout);
    traceinst(cpu, sim_args.traceinst);
    free_cpu(cpu);
}