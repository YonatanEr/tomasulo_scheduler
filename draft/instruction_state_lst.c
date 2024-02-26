#include "instruction_state_lst.h"
#include <stdlib.h>
#include <assert.h>


void free_inst_state_lst(InstStateNode* inst_state_node){
    if (inst_state_node != NULL){
        free_inst_state_lst(inst_state_node->next);
        free_instruction_state(inst_state_node->inst_state);
        free(inst_state_node);
        inst_state_node = NULL;
    }
}

int inst_state_lst_len(InstStateNode* inst_state_node){
    if (inst_state_node == NULL){
        return 0;
    }
    return 1 + inst_state_lst_len(inst_state_node->next);
}

InstStateNode* create_node(InstState* inst_state){
    InstStateNode* new_inst_state_node = (InstStateNode*) calloc (1, sizeof(InstStateNode));
    assert(new_inst_state_node);
    new_inst_state_node->inst_state = inst_state;
    new_inst_state_node->next = NULL;
    return new_inst_state_node;
}

InstStateNode* inst_state_lst_end(InstStateNode* inst_state_node){
    while (inst_state_node->next != NULL){
        inst_state_node = inst_state_node->next;
    }
    return inst_state_node;
}

void insert_inst_state(InstStateNode* inst_state_node, InstState* inst_state){
    InstStateNode* new_inst_state_node = create_node(inst_state);
    if (new_inst_state_node == NULL){
        inst_state_node = new_inst_state_node;
    }
    InstStateNode* lst_end = inst_state_lst_end(inst_state_node);
    if (lst_end == NULL){
        inst_state_node = new_inst_state_node;
        return;
    }
    inst_state_node->next = new_inst_state_node;
}