# NESemu #

NESEmu is as cross-platform Nintendo emulator writen in c++14. 

### How to compile and run. ###
#### OSX ####
 * Download and instal [XCode](https://itunes.apple.com/us/app/xcode/id497799835?mt=12)
 * Download and install Install [CMake](https://cmake.org/download/)
 * Download and install (Visual Studio Code)[https://code.visualstudio.com/Download] (optional)
 * Install [Homebrew](https://brew.sh/)
 * Open terminal, clone the repository
    *  ```brew install sdl2```
    *  ```mkdir build```

##### Compile on Terminal #####
```sh
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ..
$ make -j 8
```
##### Compile on Visual Studio Code #####
* Open terminal on the clonned directory ``` mkdir build ```
* Switch back to VSCode and go to menu task -> run task -> cmake
* Press F5 to run in debug mode

##### run #####
* Copy the rom to the build directory

```sh
$ ./nesemu YOUR_ROM_FILE.nes
```

## ROM Compatibility ##

Right now the emulator is only compatible to mapper 0 games.

>Programing is moving zeros and ones for one place to another.
Don't belive me? Check [mov is Turing-complete](https://www.cl.cam.ac.uk/~sd601/papers/mov.pdf)
