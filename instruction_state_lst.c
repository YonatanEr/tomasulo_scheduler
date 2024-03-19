#include "instruction_state_lst.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


// frees a single node
void free_inst_state_node(InstStateNode* inst_state_node){
    free_instruction_state(inst_state_node->inst_state);
    free(inst_state_node);
    inst_state_node = NULL;
}


// frees the whole lst recursively
void free_inst_state_lst(InstStateNode* inst_state_node){
    if (inst_state_node != NULL){
        free_inst_state_lst(inst_state_node->next);
        free_inst_state_node(inst_state_node);
    }
}

// returns the length of the lst
int inst_state_lst_len(InstStateNode* inst_state_node){
    return (inst_state_node == NULL)?0:1+inst_state_lst_len(inst_state_node->next);
}


// returns a pointed to the last node of the lst
InstStateNode* inst_state_lst_end(InstStateNode* inst_state_node){
    return (inst_state_node->next==NULL)?inst_state_node:inst_state_lst_end(inst_state_node->next);
}


// inserts a new instructions into the lst's end
void insert_inst_state(InstStateNode** inst_state_node, int _inst, int pc, int cycle_fetched){
    InstStateNode* new_inst_state_node = (InstStateNode*) calloc (1, sizeof(InstStateNode));
    assert(new_inst_state_node);
    new_inst_state_node->inst_state = init_instruction_state(_inst, pc, cycle_fetched);
    new_inst_state_node->next = NULL;
    if (*inst_state_node == NULL){
        *inst_state_node = new_inst_state_node;
        return;
    }
    inst_state_lst_end(*inst_state_node)->next = new_inst_state_node;
}


// prints the current instruction state lst recursively
void print_inst_state_lst(InstStateNode* inst_state_node){
    printf("        -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if (inst_state_node != NULL){
        printf("        ");
        print_inst_state(inst_state_node->inst_state);
        print_inst_state_lst(inst_state_node->next);
    }
}
