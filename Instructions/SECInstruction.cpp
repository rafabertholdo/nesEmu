#include "SECInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SECInstruction> registrar("SECInstruction");
}

vector<shared_ptr<Instruction>> SECInstruction::createInstructions() {
    auto instruction = make_shared<SECInstruction>(implict,0x38,1,"SEC");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void SECInstruction::execute(CPU& cpu, const uint_least16_t &value) {    
    Instruction::execute(cpu, value);        
    cpu.Flags.Carry = 1;
}
