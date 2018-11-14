#ifndef TYA_INSTRUCTION_H
#define TYA_INSTRUCTION_H

#include "Instruction.h"

/*
TYA - Transfer Y to Accumulator

A = Y

Copies the current contents of the Y register into the accumulator and sets the zero and negative flags as appropriate.
*/
class TYAInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif