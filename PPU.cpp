#include "PPU.h"
#include <iostream>
#include <memory>

/* Memory mapping: Convert PPU memory address into a reference to relevant data */
u8& PPU::mmap(int i) {
    i &= 0x3FFF;
    if (i >= 0x3F00) { 
        if (i % 4 == 0 ) {
            i &= 0x0F; 
        }
        return palette[i & 0x1F]; 
    }
    std::shared_ptr<u8> r(new u8);
    if (i < 0x2000) {
        //auto index1 = (i / GamePak::VROM_Granularity) % GamePak::VROM_Pages;
        //auto index2 = i % GamePak::VROM_Granularity;
        //return GamePak::Vbanks[index][index2];
        return *r.get();
    }

    //return GamePak::Nta[(i>>10)&3][i&0x3FF];
    return *r.get();
}

uint_least8_t PPU::access(uint_least16_t index, uint_least8_t value, bool write) {
    auto refreshOpenBus = [&](uint_least8_t v) { 
        return open_bus_decay_timer = 77777, open_bus = value; 
    };
    uint_least8_t res = open_bus;
    
    if (write) {
        refreshOpenBus(value);
    }

    switch(index) // Which port from $200x?
    {
            case 0: 
                if(write) { 
                    reg.sysctrl = value; 
                    scroll.basenta = reg.BaseNTA; 
                } 
                break;
            case 1: 
                if(write) { 
                    reg.dispctrl = value; 
                } 
                break;
            case 2: 
                if(!write) {   
                    res = reg.status | (open_bus & 0x1F);
                    reg.InVBlank = false;  // Reading $2002 clears the vblank flag.
                    offset_toggle = false; // Also resets the toggle for address updates.
                    if(VBlankState != -5) {
                        VBlankState = 0; // This also may cancel the setting of InVBlank.
                    }
                }
                break;
            case 3: 
                if(write) { 
                    reg.OAMaddr = value; 
                }
                break; // Index into Object Attribute Memory
            case 4: 
                if(write) {
                    OAM[reg.OAMaddr++] = value;        // Write or read the OAM (sprites).
                } else {
                    res = refreshOpenBus(OAM[reg.OAMaddr] & (reg.OAMdata==2 ? 0xE3 : 0xFF));
                }
                break;
            case 5: // Set background scrolling offset
                if(write) {   
                    if (offset_toggle) { 
                        scroll.yfine   = value & 7; 
                        scroll.ycoarse = value >> 3; 
                    } else { 
                        scroll.xscroll = value; 
                    }
                    offset_toggle = !offset_toggle;
                }
                break;
            case 6: // Set video memory position for reads/writes
                if(write) {   
                    if (offset_toggle) { 
                        scroll.vaddrlo = value; vaddr.raw = (unsigned) scroll.raw;
                    } else { 
                        scroll.vaddrhi = value & 0x3F; 
                    }
                    offset_toggle = !offset_toggle;
                }
                break;
            case 7:
                res = read_buffer;
                u8& t = mmap(vaddr.raw); // Access the video memory.
                if(write) {
                    res = t = value;
                } else { 
                    if((vaddr.raw & 0x3F00) == 0x3F00) { // palette?
                          res = read_buffer = (open_bus & 0xC0) | (t & 0x3F);
                    }
                    read_buffer = t; 
                }
                refreshOpenBus(res);
                vaddr.raw = vaddr.raw + (reg.Inc ? 32 : 1); // The address is automatically updated.
                break;
        }
        return res;
}