# NESemu #

NESEmu is as cross-platform Nintendo emulator writen in c++14.

## How to compile and run. ##
### OSX ###
 * Download and instal [XCode](https://itunes.apple.com/us/app/xcode/id497799835?mt=12)
 * Download and install [CMake](https://cmake.org/download/)
 * Download and install [Visual Studio Code](https://code.visualstudio.com/Download) (optional)
 * Install [Homebrew](https://brew.sh/)
 * Open terminal on the clonned directory
 *  ```brew install sdl2```
 *  ```mkdir build```

#### Compile on Terminal ####
```sh
$ cd build
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release ..
$ make -j 8
```
#### Compile on Visual Studio Code ####
* task -> run task -> cmake
* ⌘ + shift + b

#### run ####
* Copy the rom to the build directory

#### terminal ####

```sh
$ ./nesemu YOUR_ROM_FILE.nes
```

#### Visual Studio Code ####
* Edit the property args in the file launch.json to "build/YOUR_ROM_FILE.nes"
* Hit F5

## ROM Compatibility ##

Right now the emulator is only compatible to NTSC mapper 0 games and [Vertical Mirroring](https://wiki.nesdev.com/w/index.php/Mirroring).

* 110 in 1 Menu
* 58 In 1 Menu
* Arkanoid
* Astro Robo Sasa
* Back to the Future
* Baltron
* Bokosuka Wars
* Challenger
* Dig Dug 2 (J)
* Dough Boy
* Field Combat
* Flappy
* Four Card Games
* Golf
* Gomoku Narabe
* Gyrodine
* Hyper Olympic
* Hyper Olympic - Tonosoma Edition
* Hyper Sports
* Ice Hockey
* Ice Hockey (disk coversion)
* Ikki
* Jewelry
* Joust
* Karateka
* Kinniku Man
* Kung-Fu
* Lode Runner (J)
* Lunar Ball
* Macross
* Mappy
* Mario Bros
* Ninja Kun
* Obake no Qtarou
* Onyanko Town
* Othello (J)
* Pac-Land
* Penguin Kun Wars
* Poo-Yan
* Popeye
* Popeye Eigo Asobi
* Portpia Renzoku Satsujin Jiken
* Pyramid
* Raid on Bungling Bay (J)
* Robot Gyro (Gyromite)
* Seicross
* Seicross (J)
* Space Invaders
* Spartan X
* Spelunker (J)
* Spy Vs. Spy (J)
* Sqoon (J)
* Star Force (J)
* Star Gate
* Super Dynamix Badminton
* Super Mario Bros
* Tennis
* Tower of Druaga, The
* Urban Champion
* Volguard 2
* Wild Gunman
* Yie-Ar Kung-Fu
* Yo Nin Uti Majyan
* Zippy Race


>Programming is moving zeros and ones from one place to another.
