#include "Instructions/RLAInstruction.h"
#include "Instructions/ROLInstruction.h"
#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RLAInstruction> registrar("RLAInstruction");
}

vector<shared_ptr<Instruction>> RLAInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x27,      0x37,     0x2F,      0x3F,      0x3B,      0x23,      0x33};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};  

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<RLAInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "RLA", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}


uint_least16_t RLAInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    auto rolValue = ROLInstruction::sharedAction(cpu, value);
    return ANDInstruction::sharedAction(cpu, rolValue);
}