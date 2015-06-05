# GPU-particles
![particles in action](https://cdn.rawgit.com/Nelarius/GPU-particles/master/images/screenie.png)

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


