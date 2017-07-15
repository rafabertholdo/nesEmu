#include "Instructions/CLVInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLVInstruction> registrar("CLVInstruction");
}

vector<shared_ptr<Instruction>> CLVInstruction::createInstructions() {
    auto instruction = make_shared<CLVInstruction>(implict,0xB8,1,"CLV", AffectFlags::Overflow);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}