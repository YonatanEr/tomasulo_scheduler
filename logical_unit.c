#include "logical_unit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void init_logical_unit(LogicalUnit* logical_unit, int type){
    logical_unit->nr_avail_fus = logical_unit->nr_fus;
    ResSta* res_sta_arr = (ResSta*) calloc (logical_unit->nr_res_stas, sizeof(ResSta));
    assert(res_sta_arr);
    for (int index=0; index<logical_unit->nr_res_stas; index++){
        res_sta_arr[index] = get_reservation_stations(index, type);
    }
}

void free_logical_unit(LogicalUnit* logical_unit){
    free(logical_unit->res_sta_arr);
    logical_unit->res_sta_arr = NULL;
}
