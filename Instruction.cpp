#include "Instruction.h"
#include "Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

Instruction::Instruction() : opcode(0), length(1) {
    
}

Instruction::~Instruction() {

}

void Instruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {
    Utils<uint_least8_t>::printHex(cpu.PC);    
    std::cout << " " << menmonic;     
}

SEIInstruction::SEIInstruction() {
    opcode = 0x78;
    length = 1;   
    menmonic = "SEI";
}

void SEIInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {    
    Instruction::execute(cpu, instructionData);        
    cpu.Flags.InterruptEnabled = 0;
    cout << std::endl;
}

LDAInstruction::LDAInstruction() {
    opcode = 0xA9;
    length = 2;   
    menmonic = "LDA";
}

void LDAInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {
    Instruction::execute(cpu, instructionData);
    cpu.A = instructionData[0];
    cout << " ";
    Utils<uint_least8_t>::printHex(cpu.A);
    cout << std::endl;
}

CLDInstruction::CLDInstruction() {
    opcode = 0xD8;
    length = 1;   
    menmonic = "CLD";
}

void CLDInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {
    Instruction::execute(cpu, instructionData);
    cpu.Flags.DecimalMode = 0;
    cout << std::endl;
}

STAInstruction::STAInstruction() {
    opcode = 0x8D;
    length = 3;   
    menmonic = "STA";
}

void STAInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {
    Instruction::execute(cpu, instructionData);
    
    uint_least16_t address = Utils<uint_least8_t>::getLittleEndianValue(instructionData);
    cpu.write(address, cpu.A);

    cout << " ";   
    
    Utils<uint_least8_t>::printVector(instructionData);
    
    cout << std::endl;
}