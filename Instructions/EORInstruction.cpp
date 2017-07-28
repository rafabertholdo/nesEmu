#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<EORInstruction> registrar("EORInstruction");
    Instruction::Registrar2<EORInstruction> registrar2("EORInstruction");
}

namespace EOR {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x49,     0x45,      0x55,     0x4D,      0x5D,      0x59,      0x41,      0x51};
}

void EORInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < EOR::opcodeList.size(); i++) {        
        instructions.at(EOR::opcodeList[i]) = make_unique<EORInstruction>(EOR::addressingModeList[i], EOR::opcodeList[i], "EOR", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void EORInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < EOR::opcodeList.size(); i++) {
        Instruction instruction(EOR::addressingModeList[i], EOR::opcodeList[i], "EOR", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(EORInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t EORInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A ^= value;
}

uint_least16_t EORInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return EORInstruction::sharedAction(cpu, value);
}