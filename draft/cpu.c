#include "cpu.h"


int main(){
    CPU cpu;

    Tag tag = cpu.inst_state_lst->res_sta_tag;
    char type = tag.type;
    char res_idx = tag.res_sta_idx;
    cpu.logical_unit_arr[type].res_sta_arr[res_idx];
}