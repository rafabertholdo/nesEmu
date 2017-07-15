#ifndef BVS_INSTRUCTION_H
#define BVS_INSTRUCTION_H

#include "Instruction.h"

class BVSInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif