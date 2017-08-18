#include "Instructions/BVSInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVSInstruction> registrar("BVSInstruction");
}

void BVSInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x70;    
    insctructions[opcode] = Instruction(relative, opcode, "BVS", BVSInstruction::sharedAction);
}

uint_least16_t BVSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Overflow) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}