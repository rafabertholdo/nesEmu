#ifndef LAX_INSTRUCTION_H
#define LAX_INSTRUCTION_H

#include "Instruction.h"

/*
LAX - Load accumulator and X register with memory.
*/
class LAXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif