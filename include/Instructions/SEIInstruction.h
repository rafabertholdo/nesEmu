#ifndef SEI_INSTRUCTION_H
#define SEI_INSTRUCTION_H

#include "Instruction.h"

class SEIInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif