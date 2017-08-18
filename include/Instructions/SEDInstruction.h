#ifndef SED_INSTRUCTION_H
#define SED_INSTRUCTION_H

#include "Instruction.h"

/*
SED - Set Decimal Flag

D = 1

Set the decimal mode flag to one.
*/
class SEDInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(InstructionArray &instructions);         
        
};

#endif