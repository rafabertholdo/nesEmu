#ifndef DEX_INSTRUCTION_H
#define DEX_INSTRUCTION_H

#include "Instruction.h"

/*
DEX - Decrement X Register

X,Z,N = X-1

Subtracts one from the X register setting the zero and negative flags as appropriate.
*/
class DEXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);        
};

#endif