#include "ROM.h" 
#include "Utils.cpp" 
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

#include <cassert>

using namespace std;

static const int kPrgBlockSizeInKb = 0x4000; //16kb
static const int kChrBlockSizeInKb = 0x2000; //8kb

ROM::ROM(const char* filePath) {   

    std::cout << "Reading file: " << filePath << std::endl;
    if (readFile(filePath)) {
        // Read the ROM file header
        assert(data[0] =='N' && data[1]=='E' && data[2]=='S' && data[3]=='\32');

        prgROMSize = data[4] * kPrgBlockSizeInKb; 
        chrROMSize = data[5] * kChrBlockSizeInKb;
        ctrlbyte = data[6];
        mappernum = data[7] | (ctrlbyte >> 4); //Four higher bits of ROM Mapper Type
        if(mappernum >= 0x40) {
            mappernum &= 15;
        }        
        prgRAMSize = data[8] * kChrBlockSizeInKb;

        loadProgramData();        
    }
}

ROM::~ROM(){
    
}

const std::vector<u8>& ROM::chr() const {
    return m_chr;
}

const std::array<u32, CHR_PAGES>& ROM::chrMap() const {
    return m_chrMap;
}

const std::vector<u8>& ROM::prg() const {
    return m_prg;
}

const std::array<u32, PRG_PAGES>& ROM::prgMap() const {
    return m_prgMap;
}

void ROM::loadProgramData() {
     int prgStart = HEADER_SIZE;
     int prgEnd = prgStart + prgROMSize;        
     m_prg = vector<u8>(&data[prgStart], &data[prgEnd]);

     if (chrROMSize) {
         int chrEnd = prgEnd + chrROMSize;
         m_chr = vector<u8>(&data[prgEnd], &data[chrEnd - 1]);    
     }       
     
     init();
     std::cout << "prg size: " << m_prg.size() << std::endl;
     std::cout << "chr size: " << m_chr.size() << std::endl;
 }

bool ROM::readFile(const char* filename) {
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

void ROM::init() {
    mapPrg(64, 0, 0);
    mapChr(8, 0, 0);
}

/* PRG mapping functions */
void ROM::mapPrg(int pageKBs, int slot, int bank) {
    if (bank < 0) {
        bank = (prgROMSize / (0x400 * pageKBs)) + bank;
    }

    for (int i = 0; i < (pageKBs / 8); i++) {
        m_prgMap[(pageKBs / 8) * slot + i] = (pageKBs * 0x400 * bank + 0x2000 * i) % prgROMSize;
    }
}

/* CHR mapping functions */
void ROM::mapChr(int pageKBs, int slot, int bank) {
    for (int i = 0; i < pageKBs; i++) {
        m_chrMap[pageKBs * slot + i] = (pageKBs * 0x400 * bank + 0x400 * i) % chrROMSize;
    }
}