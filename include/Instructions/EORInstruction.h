#ifndef EOR_INSTRUCTION_H
#define EOR_INSTRUCTION_H

#include "Instruction.h"

/*
EOR - Exclusive OR

A,Z,N = A^M

An exclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
*/
class EORInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);                 
};

#endif