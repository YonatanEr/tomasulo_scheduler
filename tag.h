#ifndef TAG_H
#define TAG_H

#define NOT_INITIALZIED -1
#define ADD_FU_IDX 0
#define MULT_FU_IDX 1
#define DIV_FU_IDX 2

#include <stdbool.h>

typedef struct Tag {
    int type;
    int res_sta_idx;
} Tag;

Tag get_tag(int type, int res_sta_idx);
void print_tag(Tag tag);
bool is_equal_tag(Tag t1, Tag t2);
bool is_tag_uninitialized(Tag t1);

#endif