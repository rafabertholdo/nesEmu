#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<EORInstruction> registrar("EORInstruction");
}

vector<shared_ptr<Instruction>> EORInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x49,     0x45,      0x55,     0x4D,      0x5D,      0x59,      0x41,      0x51};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<EORInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "EOR", AffectFlags::Negative | AffectFlags::Zero);
        instruction->readsFromMemory = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t EORInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A ^= value;
}

uint_least16_t EORInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return EORInstruction::sharedAction(cpu, value);
}