#include "logical_unit.h"
#include <assert.h>
#include <stdlib.h>

void init_logical_unit(LogicalUnit logical_unit){
    logical_unit.nr_avail_fus = logical_unit.nr_fus;
    logical_unit.res_sta_arr = (ResSta*) calloc (logical_unit.nr_res_stas, sizeof(ResSta));
    assert(logical_unit.res_sta_arr);
    for (int i=0; i<logical_unit.nr_avail_res_stas; i++){
        logical_unit.res_sta_arr[i].busy = false;
    }
}