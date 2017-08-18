#include "Instructions/RTSInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<RTSInstruction> registrar("RTSInstruction");
}

void RTSInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x60;        
    insctructions[opcode] = Instruction(implict, opcode, "RTS", RTSInstruction::sharedAction);
}

uint_least16_t RTSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    cpu.tick();
    cpu.tick();
    cpu.PC = cpu.pop() + (cpu.pop() << 8) + 1;
    cpu.tick();
    return cpu.PC;
}