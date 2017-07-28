#include "Instructions/PHPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PHPInstruction> registrar("PHPInstruction");
    Instruction::Registrar2<PHPInstruction> registrar2("PHPInstruction");
}

void PHPInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x08;
    insctructions.at(opcode) = make_unique<PHPInstruction>(implict, opcode, "PHP");
}

void PHPInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x08;
    auto instruction = PHPInstruction(implict, opcode, "PHP");
    instruction.setLambda(PHPInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t PHPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    uint_least8_t flagsToPush = cpu.Flags.raw;
    flagsToPush |= 0b110000; //bit 4 and 5 is always set on push
    cpu.push(flagsToPush);
    return flagsToPush;
}

uint_least16_t PHPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return PHPInstruction::sharedAction(cpu, value);
}