#ifndef BMI_INSTRUCTION_H
#define BMI_INSTRUCTION_H

#include "Instruction.h"

/*
BMI - Branch if Minus

If the negative flag is set then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BMIInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif