#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include "tag.h"
#include <stdbool.h>


typedef struct ResSta {
    Tag tag;
    bool busy;
    float vj, vk;
    Tag qj, qk;
} ResSta;

void set_reservation_stations(ResSta* res_sta_ptr, int type, int index);
void empty_reservation_stations(ResSta* res_sta);
void print_reservatio_station(ResSta res_sta);

#endif