#include "JMPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<JMPInstruction> registrar("JMPInstruction");
}

vector<shared_ptr<Instruction>> JMPInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{absolute, indirect};
    vector<uint_least8_t> opcodeList{             0x4C,     0x6C};
    vector<uint_least8_t> lengthList{                3,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<JMPInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "JMP");
        instructions.push_back(instruction);
    }
    return instructions;
}

void JMPInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    cpu.PC = value;
}