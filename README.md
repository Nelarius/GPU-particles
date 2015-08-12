# GPU-particles
![particles in action](https://cdn.rawgit.com/Nelarius/GPU-particles/master/images/screenie.png)

A small program I wrote for the purpose of learning to use OpenGL compute shaders, and figuring out a good way to organize OpenGL code.

## Release

The release contains a build for Windows only, currently.

Camera controls: left mouse to orbit, mouse wheel to zoom, right mouse to pan.

## Build

### Linux

In the root folder, run

`make`

and then

`make resources`

to copy the relevant text files. A program called `app` will have appeared in the root.

### Windows

Assumes that you have the MinGW toolchain installed. The `COMPILER_INCLUDES` and `LINKER_INCLUDES` need to be edited so that the compiler can include the required libraries. See below.

After that is done, the build can be executed as on Linux.

## Dependencies

SDL2

GLEW


