#include "Instructions/DECInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<DECInstruction> registrar("DECInstruction");
}

void DECInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xC6,      0xD6,     0xCE,      0xDE};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "DEC", DECInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t DECInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);        
    cpu.tick();
    cpu.write(value, --valueFromMemmory);
    return valueFromMemmory;    
}
