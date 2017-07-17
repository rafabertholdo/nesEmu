#ifndef CMP_INSTRUCTION_H
#define CMP_INSTRUCTION_H

#include "Instruction.h"

class CMPInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    void updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) override;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif