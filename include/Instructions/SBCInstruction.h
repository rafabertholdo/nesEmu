#ifndef SBC_INSTRUCTION_H
#define SBC_INSTRUCTION_H

#include "Instruction.h"

class SBCInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();
};


#endif