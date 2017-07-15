#include "Instructions/CLDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLDInstruction> registrar("CLDInstruction");
}

vector<shared_ptr<Instruction>> CLDInstruction::createInstructions() {
    auto instruction = make_shared<CLDInstruction>(implict,0xD8,1,"CLD", AffectFlags::DecimalMode);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}