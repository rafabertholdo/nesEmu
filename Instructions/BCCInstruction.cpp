#include "Instructions/BCCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCCInstruction> registrar("BCCInstruction");
    Instruction::Registrar2<BCCInstruction> registrar2("BCCInstruction");
}

void BCCInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x90;    
    insctructions.at(opcode) = make_unique<BCCInstruction>(relative, opcode, "BCC");
}

void BCCInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x90;    
    auto instruction = BCCInstruction(relative, opcode, "BCC");
    instruction.setLambda(BCCInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BCCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BCCInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BCCInstruction::sharedAction(cpu, value);
}