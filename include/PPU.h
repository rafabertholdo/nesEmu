#ifndef PPU_H
#define PPU_H

#include <vector>
#include <array>
#include "RegBit.h"
#include "Utils.h"
#include "IO.h"
#include "CPU.h"
#include "ROM.h"

static constexpr int kMaxChrRomSize = 0x80000; //500kb

enum class Mirroring { VERTICAL, HORIZONTAL };

class CPU;
class ROM;

class PPU {    
    //std::array<u8, kMaxChrRomSize> m_chr; 
    //std::array<u32, CHR_PAGES> m_chrMap;

    unsigned char CIRAM[0x800];
    unsigned char *nameTable[4] = { CIRAM+0x0000, CIRAM+0x0400, CIRAM+0x0000, CIRAM+0x0400 }; //Vertical mirroring: $2000 equals $2800 and $2400 equals $2C00

    union regtype // PPU register file
    {
        u32 value;
        // Reg0 (write)             // Reg1 (write)             // Reg2 (read)
        RegBit<0,8,u32> sysctrl;    RegBit< 8,8,u32> dispctrl;  RegBit<16,8,u32> status;
        RegBit<0,2,u32> BaseNTA;    RegBit< 8,1,u32> Grayscale; RegBit<21,1,u32> SPoverflow;
        RegBit<2,1,u32> Inc;        RegBit< 9,1,u32> ShowBG8;   RegBit<22,1,u32> SP0hit;
        RegBit<3,1,u32> SPaddr;     RegBit<10,1,u32> ShowSP8;   RegBit<23,1,u32> InVBlank;
        RegBit<4,1,u32> BGaddr;     RegBit<11,1,u32> ShowBG;    // Reg3 (write)
        RegBit<5,1,u32> SPsize;     RegBit<12,1,u32> ShowSP;    RegBit<24,8,u32> OAMaddr;
        RegBit<6,1,u32> SlaveFlag;  RegBit<11,2,u32> ShowBGSP;  RegBit<24,2,u32> OAMdata;
        RegBit<7,1,u32> NMIenabled; RegBit<13,3,u32> EmpRGB;    RegBit<26,6,u32> OAMindex;
    } reg;
    // Raw memory data as read&written by the game
    u8 palette[32], OAM[256];
    // Decoded sprite information, used & changed during each scanline
    struct { u8 sprindex, y, index, attr, x; u16 pattern; } OAM2[8], OAM3[8];

    union scrolltype
    {
        RegBit<3,16,u32> raw;       // raw VRAM address (16-bit)
        RegBit<0, 8,u32> xscroll;   // low 8 bits of first write to 2005
        RegBit<0, 3,u32> xfine;     // low 3 bits of first write to 2005
        RegBit<3, 5,u32> xcoarse;   // high 5 bits of first write to 2005
        RegBit<8, 5,u32> ycoarse;   // high 5 bits of second write to 2005
        RegBit<13,2,u32> basenta;   // nametable index (copied from 2000)
        RegBit<13,1,u32> basenta_h; // horizontal nametable index
        RegBit<14,1,u32> basenta_v; // vertical   nametable index
        RegBit<15,3,u32> yfine;     // low 3 bits of second write to 2005
        RegBit<11,8,u32> vaddrhi;   // first write to 2006 (with high 2 bits set to zero)
        RegBit<3, 8,u32> vaddrlo;   // second write to 2006
    } scroll, vaddr;

    unsigned pat_addr, sprinpos, sproutpos, sprrenpos, sprtmp;
    u16 tileattr, tilepat, ioaddr;
    u32 bg_shift_pat, bg_shift_attr;

    int scanline=241, x=0, scanline_end=341, VBlankState=0, cycle_counter=0;
    int read_buffer=0, open_bus=0, open_bus_decay_timer=0;
    bool even_odd_toggle=false, offset_toggle=false;

    u8& memoryMap(int i);        
public:
    PPU();
    ~PPU();
    void rendering_tick();
    void render_pixel();
    void tick();    
    // External I/O: read or write
    uint_least8_t access(uint_least16_t index, uint_least8_t v, bool write);    
};

#endif