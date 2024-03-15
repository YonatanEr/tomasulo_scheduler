#include "tag.h"
#include "stdio.h"


Tag get_tag(int type, int res_sta_idx){
    Tag tag;
    tag.type = type;
    tag.res_sta_idx = res_sta_idx;
    return tag;
}

void print_tag(Tag t1){
    if (t1.type == -1){
        printf("type = %3d   ", t1.type);
    }
    else{
        char* idx2name[3] = {"ADD", "MUL", "DIV"};
        printf("type = %s   ", idx2name[t1.type]);
    }
    printf("res_sta_idx = %2d   ", t1.res_sta_idx);
}

bool is_equal_tag(Tag t1, Tag t2){
    return t1.res_sta_idx==t2.res_sta_idx && t1.type==t2.type;
}