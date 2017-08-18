#ifndef TSX_INSTRUCTION_H
#define TSX_INSTRUCTION_H

#include "Instruction.h"

/*
TSX - Transfer Stack Pointer to X

X = S

Copies the current contents of the stack register into the X register and sets the zero and negative flags as appropriate.
*/
class TSXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    
    static void createInstructions(InstructionArray &instructions);         
      
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};


#endif