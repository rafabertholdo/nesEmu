#include "Instructions/PHPInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<PHPInstruction> registrar("PHPInstruction");
}

void PHPInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x08;    
    insctructions[opcode] = Instruction(implict, opcode, "PHP", PHPInstruction::sharedAction);
}

uint_least16_t PHPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    uint_least8_t flagsToPush = cpu.Flags.raw;
    flagsToPush |= 0b110000; //bit 4 and 5 is always set on push
    cpu.push(flagsToPush);
    return flagsToPush;
}