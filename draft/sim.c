#include "cpu.h"
#include <stdio.h>
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
    char line [MAX_INSTRUCTION_FILE_LINE_LENGTH];
    for (char i=0; i<MIN(16 - inst_state_lst_len(cpu->inst_state_lst), FETCH_PER_CYCLE); i++){
        fgets(line, MAX_INSTRUCTION_FILE_LINE_LENGTH, memin_fp);
        int _inst = strtol(line, NULL, 16);
        continue;
        if (is_halt(_inst)){
            cpu->halt = true;
            break;
        }
        else{
            InstState* inst_state = init_instruction_state(_inst, cpu->pc++);
            insert_inst_state(cpu->inst_state_lst, inst_state);
        }
    }
}

void start_simulation(CPU* cpu, SimArgs sim_args){
    FILE* memin_fp = fopen(sim_args.memin, "r");
    do
    {
        //ex2cdb;
        //issue2cdb;
        //issue();
        //clean();
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