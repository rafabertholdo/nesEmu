#ifndef IO_H
#define IO_H

#include <vector>
#include <SDL2/SDL.h>
#include "Utils.h"
#include <memory>

class IO {
    std::vector<int> joy_current;
    std::vector<int> joy_next;
    std::vector<int> joypos;

    //The window we'll be rendering to
    SDL_Window* window;
    
    //The surface contained by the window
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;
public:
    IO();
    ~IO();
    void Init();
    void PutPixel(unsigned px,unsigned py, unsigned pixel, int offset);
    void FlushScanline(unsigned py);
    void JoyStrobe(unsigned v);
    u8 JoyRead(unsigned idx);
};

#endif