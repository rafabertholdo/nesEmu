#ifndef STA_INSTRUCTION_H
#define STA_INSTRUCTION_H

#include "Instruction.h"

/*
STA - Store Accumulator

M = A

Stores the contents of the accumulator into memory.
*/
class STAInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif