#include <cpu.h>
#include <stdio.h>


typedef struct SimArgs {
    char* cfg;
    char* memin;
    char* regout;
    char* traceinst;
    char* tracecdb;
} SimArgs;

SimArgs parse_args(char **argv){
    SimArgs sim_args;
    sim_args.cfg = argv[1];
    sim_args.memin = argv[2];
    sim_args.regout = argv[3];
    sim_args.traceinst = argv[4];
    sim_args.tracecdb = argv[5];
    return sim_args; 
}

void regout(CPU cpu, char* regout_file_path){
    FILE* fp = fopen(regout_file_path , "w");
    assert(fp);
    for (int i=0; i<REGISTERS_AMOUNT; i++){
        fprintf(fp, "%f\n", cpu.reg_state_arr[i].v);
    }
    fclose(fp);
}

int main(int argc, char **argv){
    assert(argc==6);
    SimArgs sim_args = parse_args(argv);
    CPU cpu = init_cpu(sim_args.cfg);

    free_cpu(cpu);
}