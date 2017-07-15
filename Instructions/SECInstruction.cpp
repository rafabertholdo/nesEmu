#include "Instructions/SECInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SECInstruction> registrar("SECInstruction");
}

vector<shared_ptr<Instruction>> SECInstruction::createInstructions() {
    auto instruction = make_shared<SECInstruction>(implict,0x38,1,"SEC",AffectFlags::Carry);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}