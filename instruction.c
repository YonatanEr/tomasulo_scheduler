#include "instruction.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void empty_instruction_queue(InstructionQueue instructions_queue){
    for (int i=0; i<INSTRUCTIONS_QUEUE_SIZE; i++){
        instructions_queue.instruction[i].opcode = 0;
        instructions_queue.instruction[i].dst = 0;
        instructions_queue.instruction[i].src0 = 0;
        instructions_queue.instruction[i].src1 = 0;
    }
    instructions_queue.halt = false;
}

Instruction* read_instruction(int inst){
    Instruction* instruction = (Instruction*) calloc (1, sizeof(Instruction));
    assert(instruction);
    instruction->opcode = (inst >> 24) & 0xf;
    instruction->dst    = (inst >> 20) & 0xf;
    instruction->src0   = (inst >> 16) & 0xf;
    instruction->src1   = (inst >> 12) & 0xf;
    return instruction;
}


void print_instruction(Instruction* instruction){
    if (instruction->opcode == HALT){
        printf("HALT\n");
        return;
    }
    char opcode;
    switch (instruction->opcode)
    {
    case ADD:
        opcode = '+';
        break;
    case SUB:
        opcode = '-';
        break;
    case MULT:
        opcode = '*';
        break;
    case DIV:
        opcode = '/';
        break;    
    default:
        assert(NULL);
        break;
    }
    printf("F[%d] = F[%d] %c F[%d]\n", instruction->dst, instruction->src0, opcode, instruction->src1);
}


void print_instruction_queue(InstructionQueue instruction_queue){
    
}