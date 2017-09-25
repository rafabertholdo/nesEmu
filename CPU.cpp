#include "CPU.h"
#include "PPU.h"
#include "APU.h"
#include "Utils.cpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <cassert>
#include <SDL.h>

using namespace std;

static const int kRamSize = 0x800; //2040kb
static const int kCyclesPerFrame = 29780;
static const string kTestLogFile = "build/nestest.log.txt";
static const u8 kInitialFlags = 0x24;
static const u8 kInitialStackPointer = 0xFD;
static const u16 kNMIVectorAddress = 0xFFFA;
static const u16 kResetVectorAddress = 0xFFFC;
static const u16 kBrkVectorAddress = 0xFFFE;
static const u8 kResetInstructionOpcode = 0x22;
static const u8 kNMIInstructionOpcode = 0x12;
static const u8 kBRKInstructionOpcode = 0x02;

CPU::CPU()
    : m_RAM(kRamSize) , //2k of ram
      m_totalCycles(kCyclesPerFrame)
    //  m_testLogFile(std::ifstream(kTestLogFile))
    {

    Instruction::instantiateAll(m_instructions);

    m_testing = false;

	m_remainingCycles = m_totalCycles;
    Flags.raw = kInitialFlags;
    SP = kInitialStackPointer;
    m_reset = true;
    nmi = false;
    nmiEdgeDetected = false;
    intr = false;
    m_executedInstructionsCount = 0;
    A=0;
    X=0;
    Y=0;
    PC = 0;
}

CPU::~CPU() {

}

u16 CPU::getNmiVectorValue() {
    return read(kNMIVectorAddress, 2); //nmi vector
}

u16 CPU::getResetVectorValue() {
    return read(kResetVectorAddress, 2); //reset vector
}

u16 CPU::getBrkVectorValue() {
    return read(kBrkVectorAddress, 2); //brk vector
}

void CPU::executeInstruction(Instruction &instruction) {
    u16 instructionData = 0;
    auto length = instruction.length();
    if (length > 1) {
        instructionData = read(PC+1, instruction.length() - 1);
    }

    if (m_testing) {
        string line;
        if (m_testing) {
            /*
            if (m_testLogFile.is_open()) {
                getline(m_testLogFile,line);
            } else {
                cout << "Unable to open file";
            } */
        }

        vector<u8> instructionDataVector;
        if (instruction.length() > 1) {
            for(int i=0;i<instruction.length() - 1;i++) {
                instructionDataVector.push_back(read(PC+1 +i));
            }
        }

        identify(instructionDataVector, instruction);
        test(line, instructionDataVector, instruction.menmonic());
    }

    PC += instruction.length();
    instruction.execute(*this, instructionData);
}

void CPU::run() {
    bool quit = false;

    m_remainingCycles += m_totalCycles;
    while(m_remainingCycles > 0) {

        auto instructionCode = read(PC);

        bool nmiNow = nmi;
        if(m_reset)  {
            if (m_testing) {
                PC = getResetVectorValue();
            } else {
                instructionCode = kResetInstructionOpcode;
            }
        } else if(nmiNow && !nmiEdgeDetected) {
            instructionCode = kNMIInstructionOpcode;
        } else if(intr && !Flags.InterruptDisabled) {
            instructionCode = kBRKInstructionOpcode;
        }

        if(!nmiNow) {
            nmiEdgeDetected = false;
        }

        executeInstruction(m_instructions[instructionCode]);

        m_reset = false;
    }
}

void CPU::dmaOam(const u8 &value) {
    for (unsigned b=0; b<256; ++b) {
        write(0x2004, read((value&7)*0x0100+b));
    }
}

u8 CPU::memAccess(const u16 &address, const u8 &value, const bool &write) {
    // Memory writes are turned into reads while reset is being signalled
    if(m_reset && write) return memAccess(address, 0, false);

    tick();
	if (address < 0x2000) {  // m_RAM 0x0000 ... 0x1FFF
		u8& reference = m_RAM[address & 0x7FF];
		if (!write) {
			return reference;
		}
		reference = value;
	} else if (address < 0x4000) { //PPU 0x2000 ... 0x3FFF
		return PPU::instance().access(address & 7, value, write);
	} else if (address < 0x4014) { //APU address 0x4000 ... 0x4013
		if(write) {
            APU::instance().write(address & 0x1F, value);
        }
	} else if (address == 0x4014) { // OAM DMA.
		if (write) {
            dmaOam(value);
        }
	} else if (address == 0x4015) {
        if(write) {
            APU::instance().write(0x15,value);
        } else {
            return APU::instance().read();
        }
    } else if (address == 0x4016) { // Joypad 0.
		if (write) {
			IO::instance().JoyStrobe(value);
		} else {
			return IO::instance().JoyRead(0);
		}
	} else if (address == 0x4017) { // Joypad 1.
		if (write) {
			APU::instance().write(0x17,value);
		} else {
            IO::instance().JoyRead(1);
        }
	} else { // ROM prg access 0x4018 ... 0xFFFF
		return prgAccess(address, value, write);
	}

    return 0;
}

u8 CPU::read(const u16 &address) {
    return memAccess(address, 0, false);
}

u16 CPU::read(const u16 &address, const u8 &length) {
    u16 result = 0;
    for (u8 i = 0; i < length; i++) {
        result |= read(address+i) << (i * 8); //read little endian value
    }

    return result;
}

void CPU::write(const u16 &address, const u8 &value) {
    memAccess(address, value, true);
}

void CPU::push(const u8 &value) {
    write(SP + (1 << 8), value);
    SP-=1;
}

u8 CPU::pop() {
    SP+=1;
    u8 topElement = read(SP + (1 << 8));
    return topElement;
}

void CPU::tick() {
    for(unsigned n=0; n<3; ++n) {
        PPU::instance().tick();
    }
    APU::instance().tick();

    m_remainingCycles--;
}

u8 CPU::prgAccess(const u16 &address, const u8 &value, const bool &write) {
    //0x8000 - 0xBFFF
    //0xC000 - 0xFFFF

    if (address >= PRG_START) {
        auto page = (address / PRG_GRANULARITY) % PRG_PAGES;
        auto pageOffset = address % PRG_GRANULARITY;
        ROM& rom = ROM::loadROM();
        auto& prg = rom.prg();
        auto& prgMap = rom.prgMap();
        return prg[prgMap[page] + pageOffset];
    } else {
        //TODO: PRG m_RAM
        // return prgRam[addr - 0x6000];
        return 0;
    }
}

void CPU::dumpRegs() {
    //A:00 X:00 Y:00 P:24 SP:FD CYC:  0
    std::cout << "A:";
    Utils<u8>::printHex(A);
    std::cout << " X:";
    Utils<u8>::printHex(X);
    std::cout << " Y:";
    Utils<u8>::printHex(Y);
    std::cout << " P:";
    Utils<u8>::printHex(Flags.raw);
    std::cout << " SP:";
    Utils<u8>::printHex(SP);
    //std::cout << " CYC:" << std::setw(3) << std::setfill(' ') << static_cast<int>(0) << std::endl;
	cout << std::endl;
}

void CPU::test(const string &line, const vector<u8> &instructionData, const string &menmonic) {
    std::regex rgx("(.{4})\\s*(.{9}).(.{3})\\s(.{28})A:(.{2})\\sX:(.{2})\\sY:(.{2})\\sP:(.{2})\\sSP:(.{2})\\sCYC:(.*)");
    std::smatch matches;

    if(std::regex_search(line, matches, rgx)) {

		auto expectedPC = stoul(matches[1].str(),nullptr,16);
        string expectedData =  matches[2].str();

        string expectedInstruction = matches[3].str();
        string expectedInstructionData = matches[4].str();
        auto expectedA = stoul(matches[5].str(),nullptr,16);
		auto expectedX = stoul(matches[6].str(),nullptr,16);
		auto expectedY = stoul(matches[7].str(),nullptr,16);
		auto expectedP = stoul(matches[8].str(),nullptr,16);
		auto expectedSP = stoull(matches[9].str(),nullptr,16);

        assert(PC == expectedPC);
        assert(A == expectedA);
        assert(X == expectedX);
        assert(Y == expectedY);
        assert(Flags.raw == expectedP);
        assert(SP == expectedSP);
        assert(menmonic.compare(expectedInstruction) == 0);
    } else {
        std::cout << "Match not found\n";
    }
}

void CPU::identify(const vector<u8> &instructionData, const Instruction &instruction) {
    Utils<u16>::printHex(PC);
    cout << "  ";
    Utils<u8>::printHex(instruction.opcode());
    cout << " ";

    int verboseData = 7;
    for (auto data : instructionData) {
        Utils<u8>::printHex(data);
        cout << " ";
        verboseData -= 3;
    }
    cout << std::setw(verboseData) << std::setfill(' ') << " ";
    std::cout << instruction.menmonic() << " ";
    dumpRegs();
}