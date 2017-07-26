#include "IO.h"
#include <iostream>
#include <cmath>
#include "Utils.cpp"

IO::IO() : joypadBits{0,0}, joypadIndex{0,0} {
    Init();
}

void IO::Init() {
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
        std::cout << "SDL could not initialize! SDL_Error: " <<  SDL_GetError() << std::endl;
    } else {
        //Create window
        window = SDL_CreateWindow( "Nes Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 240, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if( window == NULL ) {
            std::cout << "Window could not be created! SDL_Error: << " << SDL_GetError() << std::endl;
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
        }
    }       
}

IO::~IO() {
    SDL_FreeSurface(screenSurface);

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();
}

void IO::PutPixel(unsigned px,unsigned py, unsigned pixel, int offset) {
    // The input value is a NES color index (with de-emphasis bits).
    // We need RGB values. To produce a RGB value, we emulate the NTSC circuitry.
    // For most part, this process is described at:
    //    http://wiki.nesdev.com/w/index.php/NTSC_video
    // Incidentally, this code is shorter than a table of 64*8 RGB values.
    static unsigned palette[3][64][512] = {}, prev=~0u;
    // Caching the generated colors
    if(prev == ~0u)
        for(int o=0; o<3; ++o)
        for(int u=0; u<3; ++u)
        for(int p0=0; p0<512; ++p0)
        for(int p1=0; p1<64; ++p1)
        {
            // Calculate the luma and chroma by emulating the relevant circuits:
            auto s = "\372\273\32\305\35\311I\330D\357}\13D!}N";
            int y=0, i=0, q=0;
            for(int p=0; p<12; ++p) // 12 samples of NTSC signal constitute a color.
            {
                // Sample either the previous or the current pixel.
                int r = (p+o*4)%12, pixel = r < 8-u*2 ? p0 : p1; // Use pixel=p0 to disable artifacts.
                // Decode the color index.
                int c = pixel%16, l = c<0xE ? pixel/4 & 12 : 4, e=p0/64;
                // NES NTSC modulator (square wave between up to four voltage levels):
                int b = 40 + s[(c > 12*((c+8+p)%12 < 6)) + 2*!(0451326 >> p/2*3 & e) + l];
                // Ideal TV NTSC demodulator:
                y += b;
                i += b * int(std::cos(M_PI * p / 6) * 5909);
                q += b * int(std::sin(M_PI * p / 6) * 5909);
            }
            // Convert the YIQ color into RGB
            auto gammafix = [=](float f) { return f <= 0.f ? 0.f : std::pow(f, 2.2f / 1.8f); };
            auto clamp    = [](int v) { return v>255 ? 255 : v; };
            // Store color at subpixel precision
            if(u==2) palette[o][p1][p0] += 0x10000*clamp(255 * gammafix(y/1980.f + i* 0.947f/9e6f + q* 0.624f/9e6f));
            if(u==1) palette[o][p1][p0] += 0x00100*clamp(255 * gammafix(y/1980.f + i*-0.275f/9e6f + q*-0.636f/9e6f));
            if(u==0) palette[o][p1][p0] += 0x00001*clamp(255 * gammafix(y/1980.f + i*-1.109f/9e6f + q* 1.709f/9e6f));
        }
    // Store the RGB color into the frame buffer.
    ((u32*) screenSurface->pixels) [py * 256 + px] = palette[offset][prev%64][pixel];
    prev = pixel;
}

void IO::FlushScanline(unsigned py) {
    if(py == 239) {
        SDL_UpdateWindowSurface(window);
    } 
}

void IO::JoyStrobe(const u8 &value) {
    strobe = value;
    if (value) {
        joypadIndex[0] = 0;
        joypadIndex[1] = 0;
    }
}

bool IO::CheckBit(const u8 &var, const u8 &pos) {
    return (var) & (1 << (pos));
}

u8 IO::JoyRead(const u8 &gamePort) {
    if (strobe) { //return status of button A
        return CheckBit(joypadBits[gamePort], 0);
    }
    
    return CheckBit(joypadBits[gamePort], joypadIndex[gamePort]++);
}

void IO::JoyButtonPress(const u8 &gamePort, const JoypadButton &button) {
    joypadBits[gamePort] |= 1 << button;
}

void IO::JoyButtonRelease(const u8 &gamePort, const JoypadButton &button) {
    joypadBits[gamePort] &= ~(1 << button);
}