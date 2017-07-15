#include "Instructions/CLCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLCInstruction> registrar("CLCInstruction");
}

vector<shared_ptr<Instruction>> CLCInstruction::createInstructions() {
    auto instruction = make_shared<CLCInstruction>(implict,0x18,1,"CLC", AffectFlags::Carry);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}