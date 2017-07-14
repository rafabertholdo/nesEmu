#include "CPU.h" 
#include "SEIInstruction.h"
#include "LDAInstruction.h"
#include "CLDInstruction.h"
#include "STAInstruction.h"
#include "Utils.cpp"

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

CPU::CPU(): RAM(0x800), //2k of ram    
            addressingModes {
                {implict, make_shared<ImplictAddressing>()},
                {accumulator, make_shared<AccumulatorAddressing>()},                                
                {immediate, make_shared<ImmediateAddressing>()},
                {zeroPage, make_shared<ZeroPageAddressing>()},
                {zeroPageX, make_shared<ZeroPageXAddressing>()},
                {zeroPageY, make_shared<ZeroPageYAddressing>()},
                {absolute, make_shared<AbsoluteAddressing>()},
                {absoluteX, make_shared<AbsoluteXAddressing>()},
                {absoluteY, make_shared<AbsoluteYAddressing>()},
                {relative, make_shared<RelativeAddressing>()},
                {indirect, make_shared<IndirectAddressing>()},
                {indirectX, make_shared<IndirectXAddressing>()},
                {indirectY, make_shared<IndirectYAddressing>()}
            } { 
    
    instructionVector = Instruction::instantiateAll();    

    for(auto&& instruction : instructionVector) {
        instructionsMapping[instruction->opcode] = instruction;
    }
    
    PC = 0xC000;    
    Flags.raw = 0x34;
    SP = 0xFD;
    reset = true;

    cout << "RAM size: " << RAM.size() << endl;
}

CPU::~CPU() {

}

void CPU::loadRom(Rom &rom) {
    CPU::rom = &rom;
}

void CPU::run() {   
    while (true) {
        uint_least8_t instructionCode = read(PC);
        if (instructionsMapping.find(instructionCode) != instructionsMapping.end()) {
            shared_ptr<Instruction> instruction = instructionsMapping[instructionCode]; //fetch instruction
            
            vector<uint_least8_t> instructionData;
            if (instruction->length > 1) {
                instructionData = read(PC+1, instruction->length - 1);
            }
            u16 instructionValue = addressingModes[instruction->addressingMode]->getAddress(*this, instructionData);
            
            instruction->execute(*this, instructionValue); //execute instruction
            PC += instruction->length; 
        } else {            
            std::cout << "Instruction " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(instructionCode) << " not implemented" << std::endl;
            dumpRegs();
            return;            
        }
    }
}


uint_least8_t CPU::memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write)
{
    // Memory writes are turned into reads while reset is being signalled
    if(reset && write) return memAccess(address, 0, false);

    //tick();
    if (address < 0x2000) { 
        uint_least8_t& reference = RAM[address & 0x7FF]; 
        if(!write)
            return reference; 
        reference = value; 
    } else if (address < 0x4000) {
        return ppu.access(address&7, value, write);
    }
    //else if (address < 0x4018)
        /*
        switch(address & 0x1F)
        {
            case 0x14: // OAM DMA: Copy 256 bytes from RAM into PPU's sprite memory
                if(write) {
                    for (unsigned b=0; b<256; ++b) {
                        write(0x2004, read((v&7)*0x0100+b));
                    }
                }
                return 0;
            case 0x15: if(!write) return APU::Read();        APU::Write(0x15,value); break;
            case 0x16: if(!write) return IO::JoyRead(false); IO::JoyStrobe(value); break;
            case 0x17: if(!write) return IO::JoyRead(true);  // write:passthru
            default: if(!write) break;
                     APU::Write(address&0x1F, value);
        }*/
    else return rom->access(address, value, write);
    return 0;
}

u8 CPU::read(const u16 &address) {
    return memAccess(address, 0, false);
}

vector<u8> CPU::read(const u16 &address, const u8 &length) {
    std::vector<u8> buffer;
    buffer.reserve(length);
    for (u8 i = 0; i < length; i++) {
        buffer.push_back(read(address+i));
    }
    
    return buffer;
}

void CPU::write(const uint_least16_t &address, const uint_least8_t &value) {
    memAccess(address, value, true);
}

void CPU::dumpRegs() {
    std::cout << "A " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(A) << std::endl;
    std::cout << "X " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(X) << std::endl;
    std::cout << "Y " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Y) << std::endl;
    std::cout << "Flags " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Flags.raw) << std::endl; 
    std::cout << "SP " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(SP) << std::endl; 
}