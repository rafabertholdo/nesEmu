#include "Instructions/SAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SAXInstruction> registrar("SAXInstruction");
    Instruction::Registrar2<SAXInstruction> registrar2("SAXInstruction");
}

namespace SAX {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, indirectX};
    vector<uint_least8_t> opcodeList{             0x87,      0x97,     0x8F,      0x83};
}

void SAXInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < SAX::opcodeList.size(); i++) {        
        instructions.at(SAX::opcodeList[i]) = make_unique<SAXInstruction>(SAX::addressingModeList[i], SAX::opcodeList[i], "SAX");
    }    
}

void SAXInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < SAX::opcodeList.size(); i++) {
        Instruction instruction(SAX::addressingModeList[i], SAX::opcodeList[i], "SAX");
        instruction.setLambda(SAXInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t SAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto result = cpu.X & cpu.A;
    cpu.write(value, result);
    return result;
}

uint_least16_t SAXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return SAXInstruction::sharedAction(cpu, value);
}