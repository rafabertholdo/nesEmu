#ifndef ROM_H
#define ROM_H

#include <string>
#include <memory>
#include <vector>
#include "Utils.h"
#include <array>

// An unsigned char can store 1 Bytes (8bits) of data (0-255)
static const u8 HEADER_SIZE = 16;
static const unsigned CHR_GRANULARITY = 0x0400, CHR_PAGES = 0x2000  / CHR_GRANULARITY; //8 pages
static const unsigned PRG_GRANULARITY = 0x2000, PRG_PAGES = 0x10000 / PRG_GRANULARITY; //8 pages
static const unsigned PRG_START = 0x8000;

class ROM {        
    std::vector<u8> data;
    std::vector<u8> m_prg;    
    std::vector<u8> m_chr;    
    
    u16 prgROMSize;
    u16 prgRAMSize;
    u16 chrROMSize;

    u8 ctrlbyte;
    u8 mappernum;     

    std::array<u32, PRG_PAGES> m_prgMap = {0};
    std::array<u32, CHR_PAGES> m_chrMap = {0};

    void loadProgramData();
    bool readFile(const char* filename);
public:          
    ROM(const char* filePath);
    ~ROM();  

    void init();

    void mapPrg(int pageKBs, int slot, int bank);    
    void mapChr(int pageKBs, int slot, int bank);       
    
    const std::vector<u8>& chr() const;
    const std::array<u32, CHR_PAGES>& chrMap() const;

    const std::vector<u8>& prg() const;
    const std::array<u32, PRG_PAGES>& prgMap() const;
};

#endif