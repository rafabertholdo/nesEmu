#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ANDInstruction> registrar("ANDInstruction");
}

vector<shared_ptr<Instruction>> ANDInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x29,     0x25,      0x35,     0x2D,      0x3D,      0x39,      0x21,      0x31};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ANDInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "AND", AffectFlags::Negative | AffectFlags::Zero);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t ANDInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) { 
    return cpu.A &= value;
}

uint_least16_t ANDInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ANDInstruction::sharedAction(cpu, value);
}