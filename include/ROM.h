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
static const int kPrgBlockSizeInKb = 0x4000; //16kb
static const int kChrBlockSizeInKb = 0x2000; //8kb

static const int kMaxPrgSize = kPrgBlockSizeInKb * 9;
static const int kMaxChrSize = kChrBlockSizeInKb * 9;

class ROM {        
    static ROM& theInstance;

    std::vector<u8> data;
    static std::array<u8, kMaxPrgSize> m_prg;
    static std::array<u8, kMaxChrSize> m_chr;
    
    u16 prgROMSize;
    u16 prgRAMSize;
    u16 chrROMSize;

    u8 ctrlbyte;
    u8 mappernum;     

    static std::array<u32, PRG_PAGES> m_prgMap;
    static std::array<u32, CHR_PAGES> m_chrMap;

    void loadProgramData();
    bool readFile(const char* filename);
public:          
    ROM(const char* filePath);
    ~ROM();
    
    ROM(ROM const&)             = delete;
    void operator=(ROM const&)  = delete;

    inline static ROM& loadROM(const char* filePath = 0) {
        static ROM theInstance(filePath);
        return theInstance;
    }

    void init();

    void mapPrg(int pageKBs, int slot, int bank);    
    void mapChr(int pageKBs, int slot, int bank);       
    
    static std::array<u8, kMaxChrSize>& chr();
    static const std::array<u32, CHR_PAGES>& chrMap();

    static std::array<u8, kMaxPrgSize>& prg();
    static const std::array<u32, PRG_PAGES>& prgMap();
};

#endif