#ifndef CLV_INSTRUCTION_H
#define CLV_INSTRUCTION_H

#include "Instruction.h"

/*
CLV - Clear Overflow Flag

V = 0

Clears the overflow flag.
*/
class CLVInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(InstructionArray &instructions);         
        
};


#endif