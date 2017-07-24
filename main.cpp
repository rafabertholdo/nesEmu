#include <fstream>
#include <iostream>
#include "include/Rom.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {

  if ( argc != 2 ) {
    cout << "usage: " << argv[0] << " <filename>\n";
  } else {    
    auto io = make_shared<IO>();
    auto rom = make_shared<ROM>(argv[1]);
    auto cpu = make_shared<CPU>(io);
    auto ppu = make_shared<PPU>(io,cpu,rom);
    cpu->setPPU(ppu);
    cpu->loadRom(rom);

    // Framerate control:
    u32 frameStart, frameTime;
    const int FPS   = 60;
    const int DELAY = 1000.0f / FPS;
    SDL_Event event;

    while (true) {
      frameStart = SDL_GetTicks();
      
      while( SDL_PollEvent( &event ) != 0 ) {
          switch( event.type ){
              case SDL_KEYDOWN: {
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
                  return 0;
              default:
                  break;
          }            
      }
      cpu->run();  
      // Wait to mantain framerate:
      frameTime = SDL_GetTicks() - frameStart;
      if (frameTime < DELAY) {
          SDL_Delay((int)(DELAY - frameTime));
      }
    }    
  }  
  return 1;
}



