#include "reservation_station.h"
#include <stdlib.h>
#include <stdio.h>


void set_reservation_stations(ResSta* res_sta_ptr, int type, int index){
    printf("123\n");
    res_sta_ptr->tag = get_tag(type, index);
    res_sta_ptr->busy = false;
    res_sta_ptr->qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta_ptr->qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    printf("321\n");
}

void empty_reservation_stations(ResSta* res_sta_ptr){
    res_sta_ptr->busy = false;
    res_sta_ptr->qj = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
    res_sta_ptr->qk = get_tag(NOT_INITIALZIED, NOT_INITIALZIED);
}

void print_reservatio_station(ResSta res_sta){
    print_tag(res_sta.tag);
    printf("res_sta.busy=%s   ",res_sta.busy?"true":"false");
    printf("vj = %f   ", res_sta.vj);
    printf("vk = %f   ", res_sta.vk);
    print_tag(res_sta.qj);
    print_tag(res_sta.qj);
}