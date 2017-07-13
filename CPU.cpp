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
                {
                    implict, [](auto) {
                        return 0;
                    }            
                },
                {
                    accumulator, [&](auto) {
                        return A;
                    }            
                },
                {
                    immediate, [&](auto instructionData) {                
                        return instructionData[0];
                    }            
                },
                {
                    zeroPage, [&](auto instructionData) {                
                        return read(instructionData[0]);            
                    },
                },
                {
                    zeroPageX, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
                        return read(((rawAdrress + X) % 256));
                    },
                },
                {
                    zeroPageY, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
                        return read(((rawAdrress + Y) % 256));
                    },
                },
                {
                    absolute, [&](auto instructionData) {
                        return Utils<u8>::getLittleEndianValue(instructionData);                        
                    },
                },
                {
                    absoluteX, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
                        return rawAdrress + X;
                    },
                },
                {
                    absoluteY, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
                        return rawAdrress + Y;
                    },
                },
                {
                    relative, [&](auto instructionData) {                
                        s8 address = instructionData[0];
                        return PC + address;                
                    },
                },
                {
                    indirect, [&](auto instructionData) {                
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
                        return read(read(rawAdrress) + read(rawAdrress + 1));
                    },
                },
                {
                    indirectX, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
                        return read(read((rawAdrress + X) % 256) + read((rawAdrress + X + 1) % 256) * 256);
                    },
                },
                {
                    indirectY, [&](auto instructionData) {
                        u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
                        return read(read(rawAdrress) + read((rawAdrress + 1) % 256) * 256 + Y);
                    },
                }
            } { 
    PC = HEADER_SIZE;    
    Utils<shared_ptr<Instruction>>::appendVector(instructionVector, SEIInstruction::createInstructions());
    Utils<shared_ptr<Instruction>>::appendVector(instructionVector, LDAInstruction::createInstructions());
    Utils<shared_ptr<Instruction>>::appendVector(instructionVector, CLDInstruction::createInstructions());
    Utils<shared_ptr<Instruction>>::appendVector(instructionVector, STAInstruction::createInstructions());

    for(auto&& instruction : instructionVector) {
        instructionsMapping[instruction->opcode] = instruction;
    }
    
    Flags.raw = 0x34;
    SP = 0xFD;
    reset = true;

    cout << "RAM size: " << RAM.size() << endl;
}

CPU::~CPU() {

}

void CPU::loadRom(const Rom &rom) {
    CPU::rom = &rom;
}

void CPU::run() {   

    while (true) {
        uint_least8_t instructionCode = rom->load(PC);
        if ( instructionsMapping.find(instructionCode) != instructionsMapping.end() ) {
            shared_ptr<Instruction> instruction = instructionsMapping[instructionCode]; //fetch instruction

            //TODO:addressing            
            vector<uint_least8_t> instructionData;
            if (instruction->length > 1) {
                instructionData = rom->load(PC+1, instruction->length - 1);
            }
            u16 instructionValue = addressingModes[instruction->addressingMode](instructionData);
            
            instruction->execute(*this, instructionValue); //execute instruction      
            PC += instruction->length; 
        } else {            
            std::cout << "Instruction " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(instructionCode) << " not implemented" << std::endl;
            dumpRegs();
            return;            
        }
    }
}


uint_least8_t CPU::memAccess(uint_least16_t address, uint_least8_t value, bool write)
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
    //else return GamePak::Access(address, value, write);
    return 0;
}

u8 CPU::read(u16 address) {
    return memAccess(address, 0, false);
}

vector<u8> CPU::read(u16 address, u8 length) {
    vector<u8> sub(&RAM[address],&RAM[address + length]);    
    return sub;
}

void CPU::write(uint_least16_t address, uint_least8_t value) {
    memAccess(address, value, true);
}

void CPU::dumpRegs() {
    std::cout << "A " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(A) << std::endl;
    std::cout << "X " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(X) << std::endl;
    std::cout << "Y " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Y) << std::endl;
    std::cout << "Flags " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Flags.raw) << std::endl; 
    std::cout << "SP " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(SP) << std::endl; 
}