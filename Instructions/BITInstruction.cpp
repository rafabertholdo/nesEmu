#include "Instructions/BITInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BITInstruction> registrar("BITInstruction");
    Instruction::Registrar2<BITInstruction> registrar2("BITInstruction");
}

namespace BIT {
    vector<AddressingMode> addressingModeList{zeroPage, absolute};
    vector<uint_least8_t> opcodeList{             0x24,     0x2C};
}


void BITInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < BIT::opcodeList.size(); i++) {        
        instructions.at(BIT::opcodeList[i]) = make_unique<BITInstruction>(BIT::addressingModeList[i], BIT::opcodeList[i], "BIT", AffectFlags::None, true);
    }    
}

void BITInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < BIT::opcodeList.size(); i++) {
        Instruction instruction(BIT::addressingModeList[i], BIT::opcodeList[i], "BIT", AffectFlags::None, true);
        instruction.setLambda(BITInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t BITInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Negative = (value & 128) != 0;
    cpu.Flags.Overflow = (value & 64) != 0;
    cpu.Flags.Zero = (value & cpu.A) == 0;
    return value;
}

uint_least16_t BITInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BITInstruction::sharedAction(cpu, value);
}
