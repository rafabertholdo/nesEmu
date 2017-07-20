#include "Instructions/SLOInstruction.h"
#include "Instructions/ASLInstruction.h"
#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SLOInstruction> registrar("SLOInstruction");
}

vector<shared_ptr<Instruction>> SLOInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x07,      0x17,     0x0F,      0x1F,      0x1B,      0x03,      0x13};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};  

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<SLOInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "SLO", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t SLOInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = ASLInstruction::sharedAction(cpu, value);
    return ORAInstruction::sharedAction(cpu, aslValue);
}