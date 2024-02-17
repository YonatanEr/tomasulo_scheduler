#ifndef LOGICAL_UNIT_H
#define LOGICAL_UNIT_H

#include "reservation_station.h"
#include <stdbool.h>


typedef struct LogicalUnit {
    int nr_fus;
    int nr_avail_fus;
    int fu_delay;
    int nr_res_stas;
    int nr_avail_res_stas;
    ResSta* res_sta_arr;
} LogicalUnit;


#endif