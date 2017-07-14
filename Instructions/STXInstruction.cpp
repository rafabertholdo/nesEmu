#include "STXInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STXInstruction> registrar("STXInstruction");
}

vector<shared_ptr<Instruction>> STXInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute};
    vector<uint_least8_t> opcodeList{             0x86,      0x96,     0x8E};
    vector<uint_least8_t> lengthList{                2,         2,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<STXInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "STX");
        instructions.push_back(instruction);
    }
    return instructions;
}

void STXInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);    
    cpu.write(value, cpu.X);
}