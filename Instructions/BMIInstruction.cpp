#include "Instructions/BMIInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BMIInstruction> registrar("BMIInstruction");    
}

void BMIInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x30;    
    insctructions[opcode] = Instruction(relative, opcode, "BMI", BMIInstruction::sharedAction);
}

uint_least16_t BMIInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Negative) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}