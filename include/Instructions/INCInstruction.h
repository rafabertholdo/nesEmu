#ifndef INC_INSTRUCTION_H
#define INC_INSTRUCTION_H

#include "Instruction.h"

/*
INC - Increment Memory

M,Z,N = M+1

Adds one to the value held at a specified memory location setting the zero and negative flags as appropriate.
*/
class INCInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);                 
};

#endif