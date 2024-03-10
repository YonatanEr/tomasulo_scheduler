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

ResSta get_reservation_stations(int type, int index);
void empty_reservation_stations(ResSta* res_sta);

#endif