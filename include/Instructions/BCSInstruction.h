#ifndef BCS_INSTRUCTION_H
#define BCS_INSTRUCTION_H

#include "Instruction.h"

/*
BCS - Branch if Carry Set

If the carry flag is set then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BCSInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif