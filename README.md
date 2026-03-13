# DinoRun 
A simple runner style game using standard C and SDL.


## Building
To build you will need a C compiler, all compile instructions are in the makefile so run ```make```.
The make file is needed for a successful compile (there are some defined global variables in there) and the linking
gets somewhat complicated. 

You will aslo need **SDL2** installed along with **SDL2_ttf**.  
  
On **Windows** download SDL2 and SDL2_ttf then extrat it into the project direcotry in a file called SDL. SDL should have the following inside it ```.\SDL\include\...``` ```.\SDL\lib\...``` ```.\SDL\SDL2.dll``` ```.\SDL\SDL2_ttf.dll```  

On **Linux** you need to install the SDL packages ```SDL2``` and ```SDL2_ttf```. The project sould just work if they are installed.

## Note
This is not a finished project, it is in early development (and may stay this way for quite some time).
