#include "Instructions/PLAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PLAInstruction> registrar("PLAInstruction");
    Instruction::Registrar2<PLAInstruction> registrar2("PLAInstruction");
}

void PLAInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x68;
    insctructions.at(opcode) = make_unique<PLAInstruction>(implict, opcode, "PLA", AffectFlags::Negative | AffectFlags::Zero);
}

void PLAInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x68;
    auto instruction = PLAInstruction(implict, opcode, "PLA", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(PLAInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t PLAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    cpu.tick();
    return cpu.A = cpu.pop();
}

uint_least16_t PLAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return PLAInstruction::sharedAction(cpu, value);
}