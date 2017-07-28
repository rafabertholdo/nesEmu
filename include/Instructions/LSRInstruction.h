#ifndef LSR_INSTRUCTION_H
#define LSR_INSTRUCTION_H

#include "Instruction.h"

class LSRInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);       
};


#endif