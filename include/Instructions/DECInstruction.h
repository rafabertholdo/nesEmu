#ifndef DEC_INSTRUCTION_H
#define DEC_INSTRUCTION_H

#include "Instruction.h"

/*
DEC - Decrement Memory

M,Z,N = M-1

Subtracts one from the value held at a specified memory location setting the zero and negative flags as appropriate.
*/
class DECInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif