#ifndef RLA_INSTRUCTION_H
#define RLA_INSTRUCTION_H

#include "Instruction.h"

/*
RLA - ROL + AND

Rotate one bit left in memory, then AND accumulator with memory.
*/
class RLAInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif