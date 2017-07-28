#include "Instructions/BPLInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BPLInstruction> registrar("BPLInstruction");
    Instruction::Registrar2<BPLInstruction> registrar2("BPLInstruction");
}

void BPLInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x10;    
    insctructions.at(opcode) = make_unique<BPLInstruction>(relative, opcode, "BPL");
}

void BPLInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x10;    
    auto instruction = BPLInstruction(relative, opcode, "BPL");
    instruction.setLambda(BPLInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BPLInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Negative) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BPLInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BPLInstruction::sharedAction(cpu, value);
}