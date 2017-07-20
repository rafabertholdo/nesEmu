#include "Instructions/ISBInstruction.h"
#include "Instructions/SBCInstruction.h"
#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ISBInstruction> registrar("ISBInstruction");
}

vector<shared_ptr<Instruction>> ISBInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xE7,      0xF7,     0xEF,      0xFF,      0xFB,      0xE3,      0xF3};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ISBInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ISB", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t ISBInstruction::action(CPU& cpu, const uint_least16_t &value) {
    auto incValue = INCInstruction::sharedAction(cpu, value);    
    return SBCInstruction::sharedAction(cpu, incValue);
}