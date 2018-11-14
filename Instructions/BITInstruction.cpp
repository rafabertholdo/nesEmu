#include "Instructions/BITInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BITInstruction> registrar("BITInstruction");    
}

void BITInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, absolute};
    vector<uint_least8_t> opcodeList{             0x24,     0x2C};
    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "BIT", BITInstruction::sharedAction, AffectFlags::None, true);
    }    
}

uint_least16_t BITInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Negative = (value & 128) != 0;
    cpu.Flags.Overflow = (value & 64) != 0;
    cpu.Flags.Zero = (value & cpu.A) == 0;
    return value;
}
