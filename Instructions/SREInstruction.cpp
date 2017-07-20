#include "Instructions/SREInstruction.h"
#include "Instructions/lSRInstruction.h"
#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SREInstruction> registrar("SREInstruction");
}

vector<shared_ptr<Instruction>> SREInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x47,      0x57,     0x4F,      0x5F,      0x5B,      0x43,      0x53};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};  

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<SREInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "SRE", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t SREInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = LSRInstruction::sharedAction(cpu, value);
    return EORInstruction::sharedAction(cpu, aslValue);
}