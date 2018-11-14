#ifndef ISB_INSTRUCTION_H
#define ISB_INSTRUCTION_H

#include "Instruction.h"

/*
ISB - INC + SBC

Increase memory by one, then subtract memory from accu-mulator (with borrow)
*/
class ISBInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif