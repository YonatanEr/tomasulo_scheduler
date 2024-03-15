#ifndef INSTRUCTION_STATE_LST_H
#define INSTRUCTION_STATE_LST_H

#include "instruction_state.h"
#define MAX_INSTRUCTION_STATE_LIST_SIZE 16


typedef struct InstStateNode {
    InstState* inst_state;
    struct InstStateNode* next;
} InstStateNode;


void free_inst_state_node(InstStateNode* inst_state_node);
void free_inst_state_lst(InstStateNode* inst_state_node);
int inst_state_lst_len(InstStateNode* inst_state_node);
void insert_inst_state(InstStateNode** inst_state_node, int _inst, int pc, int cycle_fetched);
void print_inst_state_lst(InstStateNode* inst_state_node);

#endif