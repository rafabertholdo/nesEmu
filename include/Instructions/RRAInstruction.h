#ifndef RRA_INSTRUCTION_H
#define RRA_INSTRUCTION_H

#include "Instruction.h"

class RRAInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif