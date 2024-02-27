#include "instruction_state_lst.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void free_inst_state_node(InstStateNode* inst_state_node){
    free_instruction_state(inst_state_node->inst_state);
    free(inst_state_node);
    inst_state_node = NULL;
}

void free_inst_state_lst(InstStateNode* inst_state_node){
    if (inst_state_node != NULL){
        free_inst_state_lst(inst_state_node->next);
        free_inst_state_node(inst_state_node);
    }
}

int inst_state_lst_len(InstStateNode* inst_state_node){
    return (inst_state_node == NULL)?0:1+inst_state_lst_len(inst_state_node->next);
}

InstStateNode* inst_state_lst_end(InstStateNode* inst_state_node){
    return (inst_state_node->next==NULL)?inst_state_node:inst_state_lst_end(inst_state_node->next);
}

void insert_inst_state(InstStateNode** inst_state_node, InstState* inst_state){
    InstStateNode* new_inst_state_node = (InstStateNode*) malloc (1 * sizeof(InstStateNode));
    assert(new_inst_state_node);
    new_inst_state_node->inst_state = inst_state;
    new_inst_state_node->next = NULL;
    if (*inst_state_node == NULL){
        *inst_state_node = new_inst_state_node;
        return;
    }
    InstStateNode* lst_end = inst_state_lst_end(*inst_state_node);
    lst_end->next = new_inst_state_node;
}

void print_inst_state_lst(InstStateNode* inst_state_node){
    printf("---------------------------------\n");
    if (inst_state_node != NULL){
        print_inst_state(inst_state_node->inst_state);
        print_inst_state_lst(inst_state_node->next);
    }
}