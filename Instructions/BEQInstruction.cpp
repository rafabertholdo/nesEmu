#include "Instructions/BEQInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BEQInstruction> registrar("BEQInstruction");
    Instruction::Registrar2<BEQInstruction> registrar2("BEQInstruction");
}


void BEQInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xF0;
    insctructions.at(opcode) = make_unique<BEQInstruction>(relative,opcode,"BEQ");
}

void BEQInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xF0;
    auto instruction = BEQInstruction(relative,opcode,"BEQ");
    instruction.setLambda(BEQInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BEQInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Zero) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BEQInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BEQInstruction::sharedAction(cpu, value);
}