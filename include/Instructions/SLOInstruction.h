#ifndef SLO_INSTRUCTION_H
#define SLO_INSTRUCTION_H

#include "Instruction.h"

/*
SLO - ASL + ORA

Shift left one bit in memory, then OR accumulator with memory
*/
class SLOInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);     
};


#endif