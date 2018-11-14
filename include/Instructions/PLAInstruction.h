#ifndef PLA_INSTRUCTION_H
#define PLA_INSTRUCTION_H

#include "Instruction.h"

/*
PLA - Pull Accumulator

Pulls an 8 bit value from the stack and into the accumulator. The zero and negative flags are set as appropriate.
*/
class PLAInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);     
};

#endif