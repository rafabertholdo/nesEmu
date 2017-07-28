#include "Instructions/BNEInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BNEInstruction> registrar("BNEInstruction");
    Instruction::Registrar2<BNEInstruction> registrar2("BNEInstruction");
}

void BNEInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xD0;    
    insctructions.at(opcode) = make_unique<BNEInstruction>(relative, opcode, "BNE");
}

void BNEInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xD0;    
    auto instruction = BNEInstruction(relative, opcode, "BNE");
    instruction.setLambda(BNEInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BNEInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Zero) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BNEInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BNEInstruction::sharedAction(cpu, value);
}