#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include "instruction.h"


typedef struct SimulationArguments {
    char* cfg;
    char* memin;
    char* regout;
    char* traceinst;
    char* tracecdb;
} SimulationArgs;


SimulationArgs parse_args(char **argv){
    SimulationArgs sim_args;
    sim_args.cfg = argv[1];
    sim_args.memin = argv[2];
    sim_args.regout = argv[3];
    sim_args.traceinst = argv[4];
    sim_args.tracecdb = argv[5];
    return sim_args; 
}


void print_args(SimulationArgs sim_args){
    printf("sim_args.cfg = %s\n", sim_args.cfg);
    printf("sim_args.memin = %s\n", sim_args.memin);
    printf("sim_args.regout = %s\n", sim_args.regout);
    printf("sim_args.traceinst = %s\n", sim_args.traceinst);
    printf("sim_args.tracecdb = %s\n", sim_args.tracecdb);
}


void handle_instrctions(CPU* cpu){
    cpu->instructions_queue;
}


void fetch(CPU* cpu, char* memin, FILE* memin_fp){
    char* line = (char*) calloc (1, MAX_INSTRUCTION_FILE_LINE_LENGTH);
    assert(line);
    char* ptr;
    while (fgets(line, MAX_INSTRUCTION_FILE_LINE_LENGTH, memin_fp) != NULL){
        int inst = strtol(line, &ptr, 16);
        Instruction* instruction = read_instruction(inst);
        print_instruction(instruction);
        cpu->instructions_queue.halt = true;
    }
    free(line);
    line = NULL;
}


int main(int argc, char **argv){
    assert(argc==6);
    SimulationArgs sim_args = parse_args(argv);
    CPU* cpu = init_cpu(sim_args.cfg);
    FILE* memin_fp = fopen(sim_args.memin, "r");
    assert(memin_fp);
    while (!cpu->instructions_queue.halt){
        fetch(cpu, sim_args.memin, memin_fp);
        issue();
    }
    fclose(memin_fp);
    regout(cpu->registers, sim_args.regout);
    free_cpu(cpu);
    return 0;
}