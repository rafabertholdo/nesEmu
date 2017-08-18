#include "Instructions/BNEInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BNEInstruction> registrar("BNEInstruction");
}

void BNEInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xD0;        
    insctructions[opcode] = Instruction(relative, opcode, "BNE", BNEInstruction::sharedAction);
}

uint_least16_t BNEInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Zero) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}