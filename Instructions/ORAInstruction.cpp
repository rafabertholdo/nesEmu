#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ORAInstruction> registrar("ORAInstruction");
}

vector<shared_ptr<Instruction>> ORAInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x09,     0x05,      0x15,     0x0D,      0x1D,      0x19,      0x01,      0x11};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ORAInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ORA", AffectFlags::Negative | AffectFlags::Zero);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t ORAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A |= value;
}

uint_least16_t ORAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ORAInstruction::sharedAction(cpu, value);
}