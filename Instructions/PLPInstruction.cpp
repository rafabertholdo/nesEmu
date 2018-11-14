#include "Instructions/PLPInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<PLPInstruction> registrar("PLPInstruction");
}

void PLPInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x28;
    insctructions[opcode] = Instruction(implict, opcode, "PLP", PLPInstruction::sharedAction);
}

uint_least16_t PLPInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {
    cpu.tick();
    cpu.tick();
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    return cpu.Flags.raw = flagsToPop;
}
