#include "Instructions/NOPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<NOPInstruction> registrar("NOPInstruction");
}

vector<shared_ptr<Instruction>> NOPInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;
    vector<AddressingMode> addressingModeList;
    vector<uint_least8_t> opcodeList;    
    vector<uint_least8_t> lengthList;
    
    vector<uint_least8_t> implicitOpcodes{0xEA,0x1A,0x3A,0x5A,0x7A,0xDA,0xFA};
    for (u8 opcode : implicitOpcodes) {
        addressingModeList.push_back(implict);
        opcodeList.push_back(opcode);
        lengthList.push_back(1);
    }

    vector<uint_least8_t> immediateOpcodes{0x80,0x82,0x89,0xC2,0xE2};
    for (u8 opcode : immediateOpcodes) {
        addressingModeList.push_back(immediate);
        opcodeList.push_back(opcode);
        lengthList.push_back(2);
    }

    vector<uint_least8_t> zeroPageOpcodes{0x04,0x44,0x64};
    for (u8 opcode : zeroPageOpcodes) {
        addressingModeList.push_back(zeroPage);
        opcodeList.push_back(opcode);
        lengthList.push_back(2);
    }

    addressingModeList.push_back(absolute);
    opcodeList.push_back(0x0C);
    lengthList.push_back(3);
    
    vector<uint_least8_t> zeroPageXOpcodes{0x14,0x34,0x54,0x74,0xD4,0xF4};
    for (u8 opcode : zeroPageXOpcodes) {
        addressingModeList.push_back(zeroPageX);
        opcodeList.push_back(opcode);
        lengthList.push_back(2);
    }

    vector<uint_least8_t> absoluteXOpcodes{0x1C,0x3C,0x5C,0x7C,0xDC,0xFC};
    for (u8 opcode : absoluteXOpcodes) {
        addressingModeList.push_back(absoluteX);
        opcodeList.push_back(opcode);
        lengthList.push_back(3);
    }

    for(int i=0; i < opcodeList.size(); i++) {        
        auto instruction = make_shared<NOPInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "NOP");
        instruction->readsFromMemory = true;
        instructions.push_back(instruction);
    }
    return instructions;

}

uint_least16_t NOPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    return value;
}