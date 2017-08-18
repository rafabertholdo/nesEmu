#ifndef TXS_INSTRUCTION_H
#define TXS_INSTRUCTION_H

#include "Instruction.h"

/*
TXS - Transfer X to Stack Pointer

S = X

Copies the current contents of the X register into the stack register.
*/
class TXSInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);               
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif