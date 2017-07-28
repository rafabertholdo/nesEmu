#include "Instructions/PLPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PLPInstruction> registrar("PLPInstruction");
    Instruction::Registrar2<PLPInstruction> registrar2("PLPInstruction");
}

void PLPInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x28;
    insctructions.at(opcode) = make_unique<PLPInstruction>(implict, opcode, "PLP");
}

void PLPInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x28;
    auto instruction = PLPInstruction(implict, opcode, "PLP");
    instruction.setLambda(PLPInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t PLPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    cpu.tick();
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    return cpu.Flags.raw = flagsToPop;
}

uint_least16_t PLPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return PLPInstruction::sharedAction(cpu, value);
}