#ifndef TAY_INSTRUCTION_H
#define TAY_INSTRUCTION_H

#include "Instruction.h"

/*
TAY - Transfer Accumulator to Y

Y = A

Copies the current contents of the accumulator into the Y register and sets the zero and negative flags as appropriate.
*/
class TAYInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    
    static void createInstructions(InstructionArray &instructions);         
       
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);     
};


#endif