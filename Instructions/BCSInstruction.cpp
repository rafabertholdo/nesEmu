#include "Instructions/BCSInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCSInstruction> registrar("BCSInstruction");
}

void BCSInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xB0;
    insctructions[opcode] = Instruction(relative, opcode, "BCS", BCSInstruction::sharedAction);
}

uint_least16_t BCSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}