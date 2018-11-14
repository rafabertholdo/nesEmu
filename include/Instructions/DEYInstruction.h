#ifndef DEY_INSTRUCTION_H
#define DEY_INSTRUCTION_H

#include "Instruction.h"

/*
DEY - Decrement Y Register

Y,Z,N = Y-1

Subtracts one from the Y register setting the zero and negative flags as appropriate.
*/
class DEYInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif