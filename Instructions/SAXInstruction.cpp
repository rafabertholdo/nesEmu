#include "Instructions/SAXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SAXInstruction> registrar("SAXInstruction");
}

void SAXInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, indirectX};
    vector<uint_least8_t> opcodeList{             0x87,      0x97,     0x8F,      0x83};

    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "SAX", SAXInstruction::sharedAction);
    }    
}

uint_least16_t SAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto result = cpu.X & cpu.A;
    cpu.write(value, result);
    return result;
}
