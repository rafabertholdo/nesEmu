#ifndef RRA_INSTRUCTION_H
#define RRA_INSTRUCTION_H

#include "Instruction.h"

/*
RRA - ROR + ADC

Rotate one bit right in memory, then add memory to accumulator
*/
class RRAInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif