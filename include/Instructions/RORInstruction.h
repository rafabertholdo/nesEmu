#ifndef ROR_INSTRUCTION_H
#define ROR_INSTRUCTION_H

#include "Instruction.h"

/*
ROR - Rotate Right

Move each of the bits in either A or M one place to the right. Bit 7 is filled with the current value of the carry flag whilst the old bit 0 becomes the new carry flag value.
*/
class RORInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);                 
};

#endif