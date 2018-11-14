#ifndef INX_INSTRUCTION_H
#define INX_INSTRUCTION_H

#include "Instruction.h"

/*
INX - Increment X Register

X,Z,N = X+1

Adds one to the X register setting the zero and negative flags as appropriate.
*/
class INXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);     
};

#endif