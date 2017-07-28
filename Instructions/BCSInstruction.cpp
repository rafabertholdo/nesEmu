#include "Instructions/BCSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCSInstruction> registrar("BCSInstruction");
    Instruction::Registrar2<BCSInstruction> registrar2("BCSInstruction");
}

void BCSInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xB0;
    insctructions.at(opcode) = make_unique<BCSInstruction>(relative,opcode,"BCS");
}

void BCSInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xB0;
    auto instruction = BCSInstruction(relative,opcode,"BCS");
    instruction.setLambda(BCSInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BCSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BCSInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BCSInstruction::sharedAction(cpu, value);
}