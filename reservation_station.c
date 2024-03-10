#include "reservation_station.h"
#include <stdlib.h>
#include <assert.h>


ResSta get_reservation_stations(int index, int type){
    ResSta res_sta;
    res_sta.tag = get_tag(type, index);
    res_sta.busy = false;
    return res_sta;
}
