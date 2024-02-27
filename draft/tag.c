#include "tag.h"
#include "stdio.h"

bool is_equal(Tag t1, Tag t2){
    return (t1.type==t2.type) && (t1.res_sta_idx==t2.res_sta_idx);
}

void print_tag(Tag t1){
    printf("type = %d\n", t1.type);
    printf("res_sta_idx = %d\n", t1.res_sta_idx);
}