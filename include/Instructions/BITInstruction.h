#ifndef BIT_INSTRUCTION_H
#define BIT_INSTRUCTION_H

#include "Instruction.h"

class BITInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
	void updateNegative(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) override;
	void updateZero(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) override;
	void updateOverflow(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) override;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif