#include "Instructions/RRAInstruction.h"
#include "Instructions/RORInstruction.h"
#include "Instructions/ADCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RRAInstruction> registrar("RRAInstruction");
}

vector<shared_ptr<Instruction>> RRAInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x67,      0x77,     0x6F,      0x7F,      0x7B,      0x63,      0x73};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};  

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<RRAInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "RRA", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}


uint_least16_t RRAInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    auto rorValue = RORInstruction::sharedAction(cpu, value);
    return ADCInstruction::sharedAction(cpu, rorValue);
}