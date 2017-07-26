#ifndef ROL_INSTRUCTION_H
#define ROL_INSTRUCTION_H

#include "Instruction.h"

class ROLInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);
    static vector<shared_ptr<Instruction>> createInstructions();
};


#endif