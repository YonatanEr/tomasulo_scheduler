#include "reservation_station.h"
#include <stdlib.h>
#include <assert.h>


ResSta get_reservation_stations(int type, int index){
    ResSta res_sta;
    res_sta.tag = get_tag(type, index);
    res_sta.busy = false;
    res_sta.qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta.qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    return res_sta;
}

void empty_reservation_stations(ResSta* res_sta){
    res_sta->busy = false;
    res_sta->qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta->qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
}
