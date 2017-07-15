#ifndef SEC_INSTRUCTION_H
#define SEC_INSTRUCTION_H

#include "Instruction.h"

class SECInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif