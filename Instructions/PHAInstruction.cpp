#include "Instructions/PHAInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<PHAInstruction> registrar("PHAInstruction");
}

void PHAInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x48;    
    insctructions[opcode] = Instruction(implict, opcode, "PHA", PHAInstruction::sharedAction);
}

uint_least16_t PHAInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {
    cpu.tick();
    cpu.push(cpu.A);
    return cpu.A;
}
