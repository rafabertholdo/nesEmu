#ifndef CLI_INSTRUCTION_H
#define CLI_INSTRUCTION_H

#include "Instruction.h"

class CLIInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif