#include "Instructions/BVSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVSInstruction> registrar("BVSInstruction");
    Instruction::Registrar2<BVSInstruction> registrar2("BVSInstruction");
}

void BVSInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x70;    
    insctructions.at(opcode) = make_unique<BVSInstruction>(relative, opcode, "BVS");
}

void BVSInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x70;    
    auto instruction = BVSInstruction(relative, opcode, "BVS");
    instruction.setLambda(BVSInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BVSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Overflow) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BVSInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BVSInstruction::sharedAction(cpu, value);
}