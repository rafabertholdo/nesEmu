#include "Instructions/PHPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PHPInstruction> registrar("PHPInstruction");
}

vector<shared_ptr<Instruction>> PHPInstruction::createInstructions() {
    auto instruction = make_shared<PHPInstruction>(implict,0x08,1,"PHP");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t PHPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    uint_least8_t flagsToPush = cpu.Flags.raw;
    flagsToPush |= 0b110000; //bit 4 and 5 is always set on push
    cpu.push(flagsToPush);
    return flagsToPush;
}