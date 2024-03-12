#ifndef LOGICAL_UNIT_H
#define LOGICAL_UNIT_H

#include "reservation_station.h"
#include "instruction_state.h"
#include <stdbool.h>


typedef struct LogicalUnit {
    int nr_fus;
    int nr_avail_fus;
    int fu_delay;
    int nr_res_stas;
    int nr_avail_res_stas;
    ResSta* res_sta_arr;
} LogicalUnit;


void init_logical_unit(LogicalUnit** logical_unit_ptr, int type);
int get_available_res_sta_idx(LogicalUnit* logical_unit);
void free_logical_unit(LogicalUnit* logical_unit);

#endif