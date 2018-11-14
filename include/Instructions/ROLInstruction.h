#ifndef ROL_INSTRUCTION_H
#define ROL_INSTRUCTION_H

#include "Instruction.h"

/*
ROL - Rotate Left

Move each of the bits in either A or M one place to the left. Bit 0 is filled with the current value of the carry flag whilst the old bit 7 becomes the new carry flag value.
*/
class ROLInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);                 
};

#endif