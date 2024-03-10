#include "tag.h"
#include "stdio.h"


Tag get_tag(char type, int res_sta_idx){
    Tag tag;
    tag.type = type;
    tag.res_sta_idx = res_sta_idx;
    return tag;
}

void print_tag(Tag t1){
    printf("type = %d\n", t1.type);
    printf("res_sta_idx = %d\n", t1.res_sta_idx);
}