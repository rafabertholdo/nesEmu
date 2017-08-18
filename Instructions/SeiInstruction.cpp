#include "Instructions/SEIInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SEIInstruction> registrar("SEIInstruction");
}

void SEIInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x78;    
    insctructions[opcode] = Instruction(implict, opcode, "SEI", SetInstruction::sharedAction, AffectFlags::InterruptDisabled);
}