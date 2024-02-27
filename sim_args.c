#include "sim_args.h"
#include <assert.h>

SimArgs parse_args(int argc, char **argv){
    assert(argc==6);
    SimArgs sim_args;
    sim_args.cfg = argv[1];
    sim_args.memin = argv[2];
    sim_args.regout = argv[3];
    sim_args.traceinst = argv[4];
    sim_args.tracecdb = argv[5];
    return sim_args; 
}
