#ifndef CLC_INSTRUCTION_H
#define CLC_INSTRUCTION_H

#include "Instruction.h"

class CLCInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif