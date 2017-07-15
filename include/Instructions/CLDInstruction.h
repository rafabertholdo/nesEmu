#ifndef CLD_INSTRUCTION_H
#define CLD_INSTRUCTION_H

#include "Instruction.h"

class CLDInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif