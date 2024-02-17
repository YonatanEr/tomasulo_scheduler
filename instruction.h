#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>

#define ADD 2
#define SUB 3
#define MULT 4
#define DIV 5
#define HALT 6
#define MAX_INSTRUCTION_FILE_LINE_LENGTH 64
#define INSTRUCTIONS_QUEUE_SIZE 16


typedef struct Instruction {
    char opcode;
    char dst;
    char src0;
    char src1;
} Instruction;


typedef struct InstructionQueue {
    Instruction instruction [INSTRUCTIONS_QUEUE_SIZE];
    bool halt;
} InstructionQueue;


void empty_instruction_queue(InstructionQueue instructions_queue);
Instruction* read_instruction(int inst);
void print_instruction(Instruction* instruction);
void print_instruction_queue(InstructionQueue instruction_queue);


#endif