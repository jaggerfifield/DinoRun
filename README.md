# DinoRun 
A simple runner style game using standard C and SDL.


## Building
To build you will need a C compiler, all compile instructions are in the makefile so run ```make```.
The make file is needed for a successful compile (there are some defined global variables in there) and the linking
gets somewhat complicated. 

You will aslo need **SDL3** installed along with **SDL3_ttf**.  
  
On **Windows** download [SDL3](https://github.com/libsdl-org/SDL) and [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf) (im using the devel-mingw) then extrat it into the project direcotry in a file called SDL. The SDL directory should have the following inside it ```.\SDL\include\...``` ```.\SDL\lib\...``` ```.\SDL\SDL3.dll``` ```.\SDL\SDL3_ttf.dll```  

On **Linux** you need to install the SDL packages ```SDL3``` and ```SDL3_ttf``` something like ```pacman -S sdl3 sdl3_ttf``` will do. The project sould just work if they are installed.

## Note
This is not a finished project, it is in early development (and may stay this way for quite some time).
