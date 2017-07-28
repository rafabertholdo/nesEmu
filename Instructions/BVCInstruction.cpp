#include "Instructions/BVCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVCInstruction> registrar("BVCInstruction");
    Instruction::Registrar2<BVCInstruction> registrar2("BVCInstruction");
}

void BVCInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x50;    
    insctructions.at(opcode) = make_unique<BVCInstruction>(relative, opcode, "BVC");
}

void BVCInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x50;    
    auto instruction = BVCInstruction(relative, opcode, "BVC");
    instruction.setLambda(BVCInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BVCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Overflow) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BVCInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BVCInstruction::sharedAction(cpu, value);
}