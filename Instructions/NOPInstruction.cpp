#include "Instructions/NOPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<NOPInstruction> registrar("NOPInstruction");
}

vector<shared_ptr<Instruction>> NOPInstruction::createInstructions() {
    auto instruction = make_shared<NOPInstruction>(implict,0xEA,1,"NOP");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t NOPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return 0;
}