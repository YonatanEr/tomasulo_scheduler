#include "tag.h"
#include "stdio.h"


// returns a new struct of tag with type and index
Tag get_tag(int type, int res_sta_idx){
    Tag tag;
    tag.type = type;
    tag.res_sta_idx = res_sta_idx;
    return tag;
}


// prints a tag
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


// returns true iff t1 == t2
bool is_equal_tag(Tag t1, Tag t2){
    return t1.res_sta_idx==t2.res_sta_idx && t1.type==t2.type;
}


// returns true iff t1 is not initilized
bool is_tag_uninitialized(Tag t1)
{
    return is_equal_tag(t1, get_tag(NOT_INITIALZIED, NOT_INITIALZIED));
}