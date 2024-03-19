#include "reservation_station.h"
#include <stdlib.h>
#include <stdio.h>


// sets up a single reservation station
void set_reservation_stations(ResSta* res_sta_ptr, int type, int index){
    res_sta_ptr->tag = get_tag(type, index);
    res_sta_ptr->busy = false;
    res_sta_ptr->qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta_ptr->qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
}


// empty the data in a single reservation station
void empty_reservation_stations(ResSta* res_sta_ptr){
    res_sta_ptr->busy = false;
    res_sta_ptr->qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta_ptr->qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
}


// prints a reservation station
void print_reservation_station(ResSta res_sta){
    print_tag(res_sta.tag);
    printf("res_sta.busy = %s   ",res_sta.busy?"true ":"false");
    printf("vj = %10f   ", res_sta.vj);
    printf("vk = %10f   ", res_sta.vk);
    print_tag(res_sta.qj);
    print_tag(res_sta.qj);
}