#ifndef SIM_ARGS_H
#define SIM_ARGS_H

typedef struct SimArgs {
    char* cfg;
    char* memin;
    char* regout;
    char* traceinst;
    char* tracecdb;
} SimArgs;

SimArgs parse_args(int argc, char **argv);

#endif