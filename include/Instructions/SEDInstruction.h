#ifndef SED_INSTRUCTION_H
#define SED_INSTRUCTION_H

#include "Instruction.h"

class SEDInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif