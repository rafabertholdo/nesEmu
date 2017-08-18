#ifndef PHA_INSTRUCTION_H
#define PHA_INSTRUCTION_H

#include "Instruction.h"

/*
PHA - Push Accumulator

Pushes a copy of the accumulator on to the stack.
*/
class PHAInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);              
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);       
};

#endif