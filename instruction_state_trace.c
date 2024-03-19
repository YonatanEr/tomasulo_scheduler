#include "instruction_state_trace.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void free_inst_state_trace(InstStateTrace* node){
    if (node != NULL){
        free_inst_state_trace(node->next);
        free(node);
        node = NULL;
    }
}


InstStateTrace* create_trace_node(InstState inst_state){
    InstStateTrace* node = (InstStateTrace*) calloc (1, sizeof(InstStateTrace));
    assert(node);
    node->inst_state = inst_state;
    node->next = NULL;
    return node;
}


InstStateTrace* find_prev(InstStateTrace** lst, int pc){
    InstStateTrace* prev = *lst;
    InstStateTrace* cur = prev->next;
    while(cur != NULL){
        if (pc < cur->inst_state.pc){
            return prev;
        }
        prev = cur;
        cur = cur->next;
    }
    return prev;
} 


void insert_to_inst_state_trace(InstStateTrace** lst, InstState inst_state){
    InstStateTrace* new_node = create_trace_node(inst_state);
    if (*lst == NULL){
        *lst = new_node;
        return;
    }
    if ((*lst)->inst_state.pc > new_node->inst_state.pc){
        new_node->next = *lst;
        *lst = new_node;
        return;
    }
    InstStateTrace* prev = find_prev(lst, new_node->inst_state.pc);
    new_node->next = prev->next;
    prev->next = new_node;
}