# space-invaders

![Game screenshot](https://i.imgur.com/2pIu8WM.png)

A simple space invaders clone in C with raylib.

## How it was made

The game was made in pure C with [Raylib](https://www.raylib.com/). The sprites were made in [aseprite](https://www.aseprite.org/), and sound effects with [rFXGen](https://raylibtech.itch.io/rfxgen).

## How to play

There are some release binaries that you can download [here](https://github.com/hugorplobo/space-invaders/releases). At the moment, only the Windows binary is available. However, you can compile the game yourself for your operating system as long as the Raylib is available for it (Check its website).

To compile yourself, you just need to compile or download the raylib and replace the files in `include` (header file) and `lib` (lib file) folders. 

You can build for windows using MinGW with this line: `gcc main.c -o game.exe -I include/ -L lib/ -lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32`

In linux, you can use [this](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux#unity-build---the-simplest-way-to-build-a-project) to build the project (Plans to add a build file in the future).

With macOS you can read [this](https://github.com/raysan5/raylib/wiki/Working-on-macOS).

Yeah, currently the build methods are a mess, sorry for this!
