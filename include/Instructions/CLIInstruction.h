#ifndef CLI_INSTRUCTION_H
#define CLI_INSTRUCTION_H

#include "Instruction.h"

/*
CLI - Clear Interrupt Disable

I = 0

Clears the interrupt disable flag allowing normal interrupt requests to be serviced.
*/
class CLIInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(InstructionArray &instructions);         
};

#endif