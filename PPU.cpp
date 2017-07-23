#include "PPU.h"
#include <iostream>
#include <memory>

PPU::PPU(const std::shared_ptr<IO> &io, const std::shared_ptr<CPU> cpu, const std::shared_ptr<ROM> rom) {
    PPU::io = io;
    PPU::cpu = cpu;
    PPU::rom = rom;
}

PPU::~PPU() {

}

/* Memory mapping: Convert PPU memory address into a reference to relevant data */
u8& PPU::memoryMap(int address) {
    address &= 0x3FFF; //if (addess > 0x#FFF) address -= 0x3FFF;
    
	if (address < 0x2000) {// CHR-ROM/RAM. 0x0000 ... 0x1FFF
		return rom->chrAccess(address);
	} else if (address < 0x3F00) { // Nametables .0x2000 ... 0x3EFF:
		auto page = (address >> 10) & 3;
		auto offset = address & 0x3FF;
		return rom->nameTable[page][offset]; 
	} else { // Palettes:  0x3F00 ... 0x3FFF		
		if (address % 4 == 0 ) {
			address &= 0x0F;
		}
		return palette[address & 0x1F];
	}    
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
                u8& t = memoryMap(vaddr.raw); // Access the video memory.
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


void PPU::rendering_tick()
{
    bool tile_decode_mode = 0x10FFFF & (1u << (x/16)); // When x is 0..255, 320..335
     // Each action happens in two steps: 1) select memory address; 2) receive data and react on it.
    switch(x % 8)
    {
        case 2: // Point to attribute table
            ioaddr = 0x23C0 + 0x400*vaddr.basenta + 8*(vaddr.ycoarse/4) + (vaddr.xcoarse/4);
            if(tile_decode_mode) break; // Or nametable, with sprites.
        case 0: // Point to nametable
            ioaddr = 0x2000 + (vaddr.raw & 0xFFF);
            // Reset sprite data
            if(x == 0) { sprinpos = sproutpos = 0; if(reg.ShowSP) reg.OAMaddr = 0; }
            if(!reg.ShowBG) break;
            // Reset scrolling (vertical once, horizontal each scanline)
            if(x == 304 && scanline == -1) vaddr.raw = (unsigned) scroll.raw;
            if(x == 256) { vaddr.xcoarse   = (unsigned)scroll.xcoarse;
                           vaddr.basenta_h = (unsigned)scroll.basenta_h;
                           sprrenpos = 0; }
            break;
        case 1:
            if(x == 337 && scanline == -1 && even_odd_toggle && reg.ShowBG) scanline_end = 340;
            // Name table access
            pat_addr = 0x1000*reg.BGaddr + 16*memoryMap(ioaddr) + vaddr.yfine;
            if(!tile_decode_mode) break;
            // Push the current tile into shift registers.
            // The bitmap pattern is 16 bits, while the attribute is 2 bits, repeated 8 times.
            bg_shift_pat  = (bg_shift_pat  >> 16) + 0x00010000 * tilepat;
            bg_shift_attr = (bg_shift_attr >> 16) + 0x55550000 * tileattr;
            break;
        case 3:
            // Attribute table access
            if(tile_decode_mode)
            {
                tileattr = (memoryMap(ioaddr) >> ((vaddr.xcoarse&2) + 2*(vaddr.ycoarse&2))) & 3;
                // Go to the next tile horizontally (and switch nametable if it wraps)
                if(!++vaddr.xcoarse) { vaddr.basenta_h = 1-vaddr.basenta_h; }
                // At the edge of the screen, do the same but vertically
                if(x==251 && !++vaddr.yfine && ++vaddr.ycoarse == 30)
                    { vaddr.ycoarse = 0; vaddr.basenta_v = 1-vaddr.basenta_v; }
            }
            else if(sprrenpos < sproutpos)
            {
                // Select sprite pattern instead of background pattern
                auto& o = OAM3[sprrenpos]; // Sprite to render on next scanline
                memcpy(&o, &OAM2[sprrenpos], sizeof(o));
                unsigned y = (scanline) - o.y;
                if(o.attr & 0x80) y ^= (reg.SPsize ? 15 : 7);
                pat_addr = 0x1000 * (reg.SPsize ? (o.index & 0x01) : reg.SPaddr);
                pat_addr +=  0x10 * (reg.SPsize ? (o.index & 0xFE) : (o.index & 0xFF));
                pat_addr += (y&7) + (y&8)*2;
            }
            break;
        // Pattern table bytes
        case 5:
            tilepat = memoryMap(pat_addr|0);
            break;
        case 7: // Interleave the bits of the two pattern bytes
            unsigned p = tilepat | (memoryMap(pat_addr|8) << 8);
            p = (p&0xF00F) | ((p&0x0F00)>>4) | ((p&0x00F0)<<4);
            p = (p&0xC3C3) | ((p&0x3030)>>2) | ((p&0x0C0C)<<2);
            p = (p&0x9999) | ((p&0x4444)>>1) | ((p&0x2222)<<1);
            tilepat = p;
            // When decoding sprites, save the sprite graphics and move to next sprite
            if(!tile_decode_mode && sprrenpos < sproutpos)
                OAM3[sprrenpos++].pattern = tilepat;
            break;
    }
    // Find which sprites are visible on next scanline (TODO: implement crazy 9-sprite malfunction)
    switch(x>=64 && x<256 && x%2 ? (reg.OAMaddr++ & 3) : 4)
    {
        default:
            // Access OAM (object attribute memory)
            sprtmp = OAM[reg.OAMaddr];
            break;
        case 0:
            if(sprinpos >= 64) { reg.OAMaddr=0; break; }
            ++sprinpos; // next sprite
            if(sproutpos<8) OAM2[sproutpos].y        = sprtmp;
            if(sproutpos<8) OAM2[sproutpos].sprindex = reg.OAMindex;
           {int y1 = sprtmp, y2 = sprtmp + (reg.SPsize?16:8);
            if(!( scanline >= y1 && scanline < y2 ))
                reg.OAMaddr = sprinpos != 2 ? reg.OAMaddr+3 : 8;}
            break;
        case 1:
            if(sproutpos<8) OAM2[sproutpos].index = sprtmp;
            break;
        case 2:
            if(sproutpos<8) OAM2[sproutpos].attr  = sprtmp;
            break;
        case 3:
            if(sproutpos<8) OAM2[sproutpos].x = sprtmp;
            if(sproutpos<8) ++sproutpos; else reg.SPoverflow = true;
            if(sprinpos == 2) reg.OAMaddr = 8;
            break;
    }
}

void PPU::render_pixel() {
    bool edge   = u8(x+8) < 16; // 0..7, 248..255
    bool showbg = reg.ShowBG && (!edge || reg.ShowBG8);
    bool showsp = reg.ShowSP && (!edge || reg.ShowSP8);

    // Render the background
    unsigned fx = scroll.xfine, xpos = 15 - (( (x&7) + fx + 8*!!(x&7) ) & 15);

    unsigned pixel = 0, attr = 0;
    if(showbg) // Pick a pixel from the shift registers
    {
        pixel = (bg_shift_pat  >> (xpos*2)) & 3;
        attr  = (bg_shift_attr >> (xpos*2)) & (pixel ? 3 : 0);
    }
    else if( (vaddr.raw & 0x3F00) == 0x3F00 && !reg.ShowBGSP )
        pixel = vaddr.raw;

    // Overlay the sprites
    if(showsp) {
        for(unsigned sno=0; sno<sprrenpos; ++sno)
        {
            auto& s = OAM3[sno];
            // Check if this sprite is horizontally in range
            unsigned xdiff = x - s.x;
            if(xdiff >= 8) continue; // Also matches negative values
            // Determine which pixel to display; skip transparent pixels
            if(!(s.attr & 0x40)) xdiff = 7-xdiff;
            u8 spritepixel = (s.pattern >> (xdiff*2)) & 3;
            if(!spritepixel) continue;
            // Register sprite-0 hit if applicable
            if(x < 255 && pixel && s.sprindex == 0) reg.SP0hit = true;
            // Render the pixel unless behind-background placement wanted
            if(!(s.attr & 0x20) || !pixel)
            {
                attr = (s.attr & 3) + 4;
                pixel = spritepixel;
            }
            // Only process the first non-transparent sprite pixel.
            break;
        }
    }
    pixel = palette[ (attr*4 + pixel) & 0x1F ] & (reg.Grayscale ? 0x30 : 0x3F);
    io->PutPixel(x, scanline, pixel | (reg.EmpRGB << 6), cycle_counter);
}

// PPU::tick() -- This function is called 3 times per each CPU cycle.
// Each call iterates through one pixel of the screen.
// The screen is divided into 262 scanlines, each having 341 columns, as such:
//
//           x=0                 x=256      x=340
//       ___|____________________|__________|
//  y=-1    | pre-render scanline| prepare  | >
//       ___|____________________| sprites _| > Graphics
//  y=0     | visible area       | for the  | > processing
//          | - this is rendered | next     | > scanlines
//  y=239   |   on the screen.   | scanline | >
//       ___|____________________|______
//  y=240   | idle
//       ___|_______________________________
//  y=241   | vertical blanking (idle)
//          | 20 scanlines long
//  y=260___|____________________|__________|
//
// On actual PPU, the scanline begins actually before x=0, with
// sync/colorburst/black/background color being rendered, and
// ends after x=256 with background/black being rendered first,
// but in this emulator we only care about the visible area.
//
// When background rendering is enabled, scanline -1 is
// 340 or 341 pixels long, alternating each frame.
// In all other situations the scanline is 341 pixels long.
// Thus, it takes 89341 or 89342 PPU::tick() calls to render 1 frame.
void PPU::tick()
{
    // Set/clear vblank where needed
    switch(VBlankState)
    {
        case -5: reg.status = 0; break;
        case 2: reg.InVBlank = true; break;
        case 0: cpu->nmi = reg.InVBlank && reg.NMIenabled; break;
    }
    if(VBlankState != 0) VBlankState += (VBlankState < 0 ? 1 : -1);
    if(open_bus_decay_timer) if(!--open_bus_decay_timer) open_bus = 0;
    // Graphics processing scanline?
    if(scanline < 240)
    {
        /* Process graphics for this cycle */
        if(reg.ShowBGSP) rendering_tick();
        if(scanline >= 0 && x < 256) render_pixel();
    }
    // Done with the cycle. Check for end of scanline.
    if(++cycle_counter == 3) cycle_counter = 0; // For NTSC pixel shifting
    if(++x >= scanline_end)
    {
        // Begin new scanline
        io->FlushScanline(scanline);
        scanline_end = 341;
        x            = 0;
        // Does something special happen on the new scanline?
        switch(scanline += 1)
        {
            case 261: // Begin of rendering
                scanline = -1; // pre-render line
                even_odd_toggle = !even_odd_toggle;
                // Clear vblank flag
                VBlankState = -5;
                break;
            case 241: // Begin of vertical blanking
                SDL_Event event;
                while( SDL_PollEvent( &event ) != 0 ) {
                    /*
                    auto keystate = SDL_GetKeyboardState(NULL);
                    io->Joy1Write( keystate[SDLK_d] ? 1 : 0 +
                                  (keystate[SDLK_a] ? 1 << 1 : 0) +
                                  (keystate[SDLK_s] ? 1 << 2 : 0) +
                                  (keystate[SDLK_w] ? 1 << 3 : 0) +
                                  (keystate[SDLK_e] ? 1 << 4 : 0) +
                                  (keystate[SDLK_q] ? 1 << 5 : 0) +
                                  (keystate[SDLK_h] ? 1 << 6 : 0) +
                                  (keystate[SDLK_j] ? 1 << 7 : 0));
                    */
                    switch( event.type ){
                        
                        case SDL_KEYDOWN: {
                            u8 joyValue = 0;
                        //Set the proper message surface
                            switch( event.key.keysym.sym ) {
                                case SDLK_j: io->JoyButtonPress(0, BUTTON_A); break;
                                case SDLK_h: io->JoyButtonPress(0, BUTTON_B); break;
                                case SDLK_q: io->JoyButtonPress(0, BUTTON_SELECT); break;
                                case SDLK_e: io->JoyButtonPress(0, BUTTON_START); break;
                                case SDLK_w: io->JoyButtonPress(0, BUTTON_UP); break;
                                case SDLK_s: io->JoyButtonPress(0, BUTTON_DOWN); break;
                                case SDLK_a: io->JoyButtonPress(0, BUTTON_LEFT); break;
                                case SDLK_d: io->JoyButtonPress(0, BUTTON_RIGHT); break;
                            }
                            ;
                            break;
                        }
                        case SDL_KEYUP:
                            switch( event.key.keysym.sym )
                            {
                                case SDLK_j: io->JoyButtonRelease(0, BUTTON_A); break;
                                case SDLK_h: io->JoyButtonRelease(0, BUTTON_B); break;
                                case SDLK_q: io->JoyButtonRelease(0, BUTTON_SELECT); break;
                                case SDLK_e: io->JoyButtonRelease(0, BUTTON_START); break;
                                case SDLK_w: io->JoyButtonRelease(0, BUTTON_UP); break;
                                case SDLK_s: io->JoyButtonRelease(0, BUTTON_DOWN); break;
                                case SDLK_a: io->JoyButtonRelease(0, BUTTON_LEFT); break;
                                case SDLK_d: io->JoyButtonRelease(0, BUTTON_RIGHT); break;
                            }                    
                            break;
                        case SDL_QUIT:
                            cpu->running = false;
                        default:
                            break;
                    }            
                }  
                // I cheat here: I did not bother to learn how to use SDL events,
                // so I simply read button presses from a movie file, which happens
                // to be a TAS, rather than from the keyboard or from a joystick.
                /*
                static FILE* fp = fopen(inputfn, "rb");
                if(fp)
                {
                    static unsigned ctrlmask = 0;
                    if(!ftell(fp))
                    {
                        fseek(fp, 0x05, SEEK_SET);
                        ctrlmask = fgetc(fp);
                        fseek(fp, 0x90, SEEK_SET); // Famtasia Movie format.
                    }
                    if(ctrlmask & 0x80) { IO::joy_next[0] = fgetc(fp); if(feof(fp)) IO::joy_next[0] = 0; }
                    if(ctrlmask & 0x40) { IO::joy_next[1] = fgetc(fp); if(feof(fp)) IO::joy_next[1] = 0; }
                }
                */
                // Set vblank flag
                VBlankState = 2;
        }
    }
}