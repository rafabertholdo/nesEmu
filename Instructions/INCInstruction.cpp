#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<INCInstruction> registrar("INCInstruction");
}

void INCInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xE6,      0xF6,     0xEE,      0xFE};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "INC", INCInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
    }    
}

uint_least16_t INCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);   
    cpu.tick();     
    cpu.write(value, ++valueFromMemmory);
    return valueFromMemmory;    
}
