#include "Instructions/BITInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BITInstruction> registrar("BITInstruction");
}

vector<shared_ptr<Instruction>> BITInstruction::createInstructions() {

    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, absolute};
    vector<uint_least8_t> opcodeList{             0x24,     0x2C};
    vector<uint_least8_t> lengthList{                2,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<BITInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "BIT", AffectFlags::Negative | AffectFlags::Overflow | AffectFlags::Zero);
        instruction->printsActionValue = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t BITInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.read(value);
}

void BITInstruction::updateNegative(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
	cpu.Flags.Negative = (actionValue & 128) != 0;
}

void BITInstruction::updateOverflow(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
	cpu.Flags.Overflow = (actionValue & 64) != 0;
}

void BITInstruction::updateZero(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
	cpu.Flags.Zero = (actionValue & cpu.A) == 0;
}

