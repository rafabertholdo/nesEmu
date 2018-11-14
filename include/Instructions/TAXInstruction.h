#ifndef TAX_INSTRUCTION_H
#define TAX_INSTRUCTION_H

#include "Instruction.h"

/*
TAX - Transfer Accumulator to X

X = A

Copies the current contents of the accumulator into the X register and sets the zero and negative flags as appropriate.
*/
class TAXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);     
};

#endif