#ifndef BVS_INSTRUCTION_H
#define BVS_INSTRUCTION_H

#include "Instruction.h"

/*
BVS - Branch if Overflow Set

If the overflow flag is set then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BVSInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif