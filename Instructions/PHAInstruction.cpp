#include "Instructions/PHAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PHAInstruction> registrar("PHAInstruction");
    Instruction::Registrar2<PHAInstruction> registrar2("PHAInstruction");
}

void PHAInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x48;
    insctructions.at(opcode) = make_unique<PHAInstruction>(implict, opcode, "PHA");
}

void PHAInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x48;
    auto instruction = PHAInstruction(implict, opcode, "PHA");
    instruction.setLambda(PHAInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t PHAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    cpu.push(cpu.A);
    return cpu.A;
}

uint_least16_t PHAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return PHAInstruction::sharedAction(cpu, value);
}