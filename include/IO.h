#ifndef IO_H
#define IO_H

#include <vector>
#include <SDL.h>
#include "Utils.h"
#include <memory>
#include "Timer.h"

enum JoypadButton {
    BUTTON_A,    
    BUTTON_B,        
    BUTTON_SELECT,   
    BUTTON_START,    
    BUTTON_UP,       
    BUTTON_DOWN,     
    BUTTON_LEFT,    
    BUTTON_RIGHT    
};

class IO {
    std::vector<u8> joypadBits;
    std::vector<u8> joypadIndex;
    bool strobe;

    //The window we'll be rendering to
    SDL_Window* window;
    
    //The surface contained by the window
    SDL_Surface* screenSurface;
    bool CheckBit(const u8 &var, const u8 &pos);
    Timer timer;
    int frameCount = 0;
public:
    IO();
    ~IO();
    
    IO(IO const&)              = delete;
    void operator=(IO const&)  = delete;
    
    inline static IO& instance() {
        static IO theInstance;
        return theInstance;
    }
    
    void PutPixel(unsigned px,unsigned py, unsigned pixel, int offset);
    void FlushScanline(unsigned py);
    
    //reset read index to 0 when strobe is 1
    void JoyStrobe(const u8 &value);

    //when strobe is 1, return status of A button 
    //when strobe is 0 nes will make 8 consecutive reads to check if buttons are pressed in the folloing order 
    //A, B, Select, Start, Up, Down, Left, Right.
    u8 JoyRead(const u8 &gamePort);
    
    //bit:   	 7     6     5     4     3     2     1     0
    //button:  Right  Left  Down  Up  Start Select   B     A
    void JoyButtonPress(const u8 &gamePort, const JoypadButton &value);
    void JoyButtonRelease(const u8 &gamePort, const JoypadButton &value);
};

#endif
