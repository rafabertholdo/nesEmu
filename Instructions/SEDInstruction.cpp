#include "Instructions/SEDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SEDInstruction> registrar("SEDInstruction");
}

vector<shared_ptr<Instruction>> SEDInstruction::createInstructions() {
    auto instruction = make_shared<SEDInstruction>(implict,0xF8,1,"SED", AffectFlags::DecimalMode);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}