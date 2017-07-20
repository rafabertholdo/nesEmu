#include "Rom.h" 
#include "Utils.cpp" 
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

#include <cassert>

static const int kPrgBlockSizeInKb = 0x4000; //16kb
static const int kChrBlockSizeInKb = 0x2000; //8kb

class Rom::impl {
public:
    vector<u8> data;
    vector<u8> rom;    
    vector<u8> chr;    
    u16 prgROMSize;
    u16 prgRAMSize;
    u16 chrROMSize;
    u8 ctrlbyte;
    u8 mappernum;
    
    void loadProgramData() {
        int prgStart = HEADER_SIZE;
        int prgEnd = prgStart + prgROMSize - 1;
        
        int chrStart = prgEnd + 1;
        int chrEnd = chrStart + chrROMSize - 1;
        
        rom = vector<u8>(&data[prgStart], &data[prgEnd]);
        chr = vector<u8>(&data[chrStart], &data[chrEnd]);

        std::cout << "prg length: " << prgROMSize << std::endl;
        std::cout << "chr length: " << chrROMSize << std::endl;
    }

    bool readFile(const char* filename) {
        // open the file:
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            // Stop eating new lines in binary mode!!!
            file.unsetf(std::ios::skipws);

            // get its size:
            std::streampos fileSize;

            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            // reserve capacity
            data.reserve(fileSize);

            // read the data:
            data.insert(data.begin(),
                    std::istream_iterator<u8>(file),
                    std::istream_iterator<u8>());
            return true;
        }
        return false;
    }
};

Rom::Rom(const char* filePath): pimpl{std::make_unique<impl>()} {
    std::cout << "Reading file: " << filePath << std::endl;
    if (pimpl->readFile(filePath)) {
        // Read the ROM file header
        assert(pimpl->data[0] =='N' && pimpl->data[1]=='E' && pimpl->data[2]=='S' && pimpl->data[3]=='\32');

        pimpl->prgROMSize = pimpl->data[4] * kPrgBlockSizeInKb; 
        pimpl->chrROMSize = pimpl->data[5] * kChrBlockSizeInKb;
        pimpl->ctrlbyte = pimpl->data[6];
        pimpl->mappernum = pimpl->data[7] | (pimpl->ctrlbyte>>4); //Four higher bits of ROM Mapper Type
        if(pimpl->mappernum >= 0x40) {
            pimpl->mappernum &= 15;
        }        
        pimpl->prgRAMSize = pimpl->data[8] * kChrBlockSizeInKb;

        pimpl->loadProgramData();
    }
}

Rom::~Rom(){
    
}

uint_least8_t Rom::access(const uint_least16_t &address, const uint_least8_t &value, const bool &write) {
    //0x8000 - 0xBFFF
    //0xC000 - 0xFFFF
    return pimpl->rom[static_cast<int>(address) - (address < 0xC000 ? 0x8000 : 0xC000)];
}


/*
uint_least8_t Rom::load(uint_least8_t address) const {
    return pimpl->data[address];
}

vector<uint_least8_t> Rom::load(uint_least8_t address, uint_least8_t length) const {
    vector<uint_least8_t> sub(&pimpl->data[address],&pimpl->data[address + length]);    
    return sub;
}*/