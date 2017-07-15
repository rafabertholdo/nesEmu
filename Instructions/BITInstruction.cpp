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
        auto instruction = make_shared<BITInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "BIT");
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t BITInstruction::action(CPU& cpu, const uint_least16_t &value) {
    uint_least8_t ramValue = cpu.read(value);
    cpu.Flags.Negative = (ramValue % 128) != 0;
    cpu.Flags.Overvlow = (ramValue &  64) != 0;
    cpu.Flags.Zero = (ramValue & cpu.A) == 0;
    return 0;
}