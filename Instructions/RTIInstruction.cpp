#include "Instructions/RTIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RTIInstruction> registrar("RTIInstruction");
    Instruction::Registrar2<RTIInstruction> registrar2("RTIInstruction");
}

void RTIInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x40;    
    insctructions.at(opcode) = make_unique<RTIInstruction>(implict, opcode, "RTI");
}

void RTIInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x40;    
    auto instruction = RTIInstruction(implict, opcode, "RTI");
    instruction.setLambda(RTIInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t RTIInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    cpu.Flags.raw = flagsToPop;
    //cpu.Flags.InterruptDisabled = 0;
    return cpu.PC = cpu.pop() + (cpu.pop() << 8);
}

uint_least16_t RTIInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return RTIInstruction::sharedAction(cpu, value);
}