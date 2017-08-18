#ifndef BPL_INSTRUCTION_H
#define BPL_INSTRUCTION_H

#include "Instruction.h"

/*
BPL - Branch if Positive

If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BPLInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);       
};

#endif