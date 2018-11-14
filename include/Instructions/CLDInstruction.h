#ifndef CLD_INSTRUCTION_H
#define CLD_INSTRUCTION_H

#include "Instruction.h"

/*
CLD - Clear Decimal Mode

D = 0

Sets the decimal mode flag to zero.
*/
class CLDInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(InstructionArray &instructions);                      
};

#endif