#include "logical_unit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void init_logical_unit(LogicalUnit** logical_unit_ptr, int type){
    LogicalUnit* logical_unit = *logical_unit_ptr;
    logical_unit->nr_avail_fus = logical_unit->nr_fus;
    logical_unit->nr_avail_res_stas = logical_unit->nr_res_stas;
    logical_unit->res_sta_arr = (ResSta*) calloc (logical_unit->nr_res_stas, sizeof(ResSta));
    assert(logical_unit->res_sta_arr);
    for (int index=0; index<logical_unit->nr_res_stas; index++){
        printf("index = %d\n", index);
        set_reservation_stations(&(logical_unit->res_sta_arr[index]), type, index);
        printf("res_sta_arr[index].busy = %d\n", logical_unit->res_sta_arr[index].busy);
    }
}

int get_available_res_sta_idx(LogicalUnit* logical_unit){
    for (int i=0; i<logical_unit->nr_res_stas; i++){
        printf("i = %d\n", i);
        printf("logical_unit->nr_res_stas = %d\n", logical_unit->nr_res_stas);
        if(!logical_unit->res_sta_arr[i].busy){
            return i;
        }
    }
    assert(NULL);
}

void free_logical_unit(LogicalUnit* logical_unit){
    free(logical_unit->res_sta_arr);
    logical_unit->res_sta_arr = NULL;
}
