#include "Instructions/SAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SAXInstruction> registrar("SAXInstruction");
}

vector<shared_ptr<Instruction>> SAXInstruction::createInstructions() {
vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, indirectX};
    vector<uint_least8_t> opcodeList{             0x87,      0x97,     0x8F,      0x83};
    vector<uint_least8_t> lengthList{                2,         2,        3,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<SAXInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "SAX");        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t SAXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    auto result = cpu.X & cpu.A;
    cpu.write(value, result);
    return result;
}