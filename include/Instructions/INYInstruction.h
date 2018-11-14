#ifndef INY_INSTRUCTION_H
#define INY_INSTRUCTION_H

#include "Instruction.h"

/*
INY - Increment Y Register

Y,Z,N = Y+1

Adds one to the Y register setting the zero and negative flags as appropriate.
*/
class INYInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif