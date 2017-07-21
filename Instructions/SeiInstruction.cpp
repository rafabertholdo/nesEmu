#include "Instructions/SEIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SEIInstruction> registrar("SEIInstruction");
}

vector<shared_ptr<Instruction>> SEIInstruction::createInstructions() {
    auto instruction = make_shared<SEIInstruction>(implict,0x78,1,"SEI", AffectFlags::InterruptDisabled);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}