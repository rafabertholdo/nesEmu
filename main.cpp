#include <fstream>
#include <iostream>
#include "ROM.h"
#include "CPU.h"
#include "IO.h"
#include <vector>
#include <SDL.h>

using namespace std;

int main(int argc, char *argv[]) {

  if ( argc != 2 ) {
    cout << "usage: " << argv[0] << " <filename>\n";
  } else {
    ROM::loadROM(argv[1]);
    IO& io = IO::instance();
    CPU& cpu = CPU::instance();

    // Framerate control:
    u32 frameStart, frameTime;
    const float FPS   = 60.098814;
    const float DELAY = 1000.0f / FPS;
    SDL_Event event;
    if (SDL_NumJoysticks() > 0) {
        SDL_Joystick *joystick = SDL_JoystickOpen(0);
        std::cout << "Number of axes: " << SDL_JoystickNumAxes(joystick) << std::endl;
        std::cout << "Number of buttons: " << SDL_JoystickNumButtons(joystick) << std::endl;
    }

    while (true) {
      frameStart = SDL_GetTicks();

      while( SDL_PollEvent( &event ) != 0 ) {
          switch( event.type ){
              case SDL_JOYAXISMOTION: {
                  //std::cout << "jaxis: " << (int)event.jaxis.axis << std::endl;
                  //std::cout << "value: " << (int)event.jaxis.value << std::endl;
                  if ((int)event.jaxis.axis) {
                      auto axisValue = (int)event.jaxis.value;
                      if (axisValue == 128) {
                          io.JoyButtonRelease(0, BUTTON_UP);
                          io.JoyButtonRelease(0, BUTTON_DOWN);
                      } else if (axisValue > 128) {
                          io.JoyButtonPress(0, BUTTON_DOWN);
                      } else {
                          io.JoyButtonPress(0, BUTTON_UP);
                      }
                  } else  {
                      auto axisValue = (int)event.jaxis.value;
                      if (axisValue == 128) {
                          io.JoyButtonRelease(0, BUTTON_LEFT);
                          io.JoyButtonRelease(0, BUTTON_RIGHT);
                      } else if (axisValue > 128) {
                          io.JoyButtonPress(0, BUTTON_RIGHT);
                      } else {
                          io.JoyButtonPress(0, BUTTON_LEFT);
                      }
                  }

                  break;
              }
              case SDL_JOYBUTTONDOWN: {
                  //std::cout << (int)event.jbutton.button << std::endl;
                  switch((int)event.jbutton.button) {
                      case 1: io.JoyButtonPress(0, BUTTON_A); break;
                      case 4: io.JoyButtonPress(0, BUTTON_B); break;
                      case 10: io.JoyButtonPress(0, BUTTON_SELECT); break;
                      case 11: io.JoyButtonPress(0, BUTTON_START); break;
                  }
                  break;
              }
              case SDL_JOYBUTTONUP:
                  switch( (int)event.jbutton.button ) {
                      case 1: io.JoyButtonRelease(0, BUTTON_A); break;
                      case 4: io.JoyButtonRelease(0, BUTTON_B); break;
                      case 10: io.JoyButtonRelease(0, BUTTON_SELECT); break;
                      case 11: io.JoyButtonRelease(0, BUTTON_START); break;
                  }
                  break;
              case SDL_KEYDOWN: {
                  switch( event.key.keysym.sym ) {
                      case SDLK_j: io.JoyButtonPress(0, BUTTON_A); break;
                      case SDLK_h: io.JoyButtonPress(0, BUTTON_B); break;
                      case SDLK_q: io.JoyButtonPress(0, BUTTON_SELECT); break;
                      case SDLK_e: io.JoyButtonPress(0, BUTTON_START); break;
                      case SDLK_w: io.JoyButtonPress(0, BUTTON_UP); break;
                      case SDLK_s: io.JoyButtonPress(0, BUTTON_DOWN); break;
                      case SDLK_a: io.JoyButtonPress(0, BUTTON_LEFT); break;
                      case SDLK_d: io.JoyButtonPress(0, BUTTON_RIGHT); break;
                  }
                  break;
              }
              case SDL_KEYUP:
                  switch( event.key.keysym.sym )
                  {
                      case SDLK_j: io.JoyButtonRelease(0, BUTTON_A); break;
                      case SDLK_h: io.JoyButtonRelease(0, BUTTON_B); break;
                      case SDLK_q: io.JoyButtonRelease(0, BUTTON_SELECT); break;
                      case SDLK_e: io.JoyButtonRelease(0, BUTTON_START); break;
                      case SDLK_w: io.JoyButtonRelease(0, BUTTON_UP); break;
                      case SDLK_s: io.JoyButtonRelease(0, BUTTON_DOWN); break;
                      case SDLK_a: io.JoyButtonRelease(0, BUTTON_LEFT); break;
                      case SDLK_d: io.JoyButtonRelease(0, BUTTON_RIGHT); break;
                  }
                  break;
              case SDL_QUIT:
                  return 0;
              default:
                  break;
          }
      }
      cpu.run();
      // Wait to mantain framerate:
      frameTime = SDL_GetTicks() - frameStart;
      if (frameTime < DELAY) {
          SDL_Delay((int)(DELAY - frameTime));
      }
    }
  }
  return 1;
}



