#ifndef TXA_INSTRUCTION_H
#define TXA_INSTRUCTION_H

#include "Instruction.h"

/*
TXA - Transfer X to Accumulator

A = X

Copies the current contents of the X register into the accumulator and sets the zero and negative flags as appropriate.
*/
class TXAInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    
    static void createInstructions(InstructionArray &instructions);         
      
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};


#endif