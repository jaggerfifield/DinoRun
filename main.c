#ifdef WIN
#include <SDL.h>
#include <SDL_ttf.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "jio.h"
#include "jdata.h"
#include "data.h"
#include "menu.h"
#include "update.h"

char tmp[256];

bool init_systems();
void get_display(void);

int main(int argc, char* argv[]){
	
	// Enable color in terminal for windows
	#ifdef WIN
	colorize();
	#endif

	// Create game files/logs
	init_files();
	
	if(!init_systems())
		return 0;

	info("Start program!");

	// Pick window size based on desktop
	SDL_DisplayMode mode;
	if(SDL_GetDesktopDisplayMode(0, &mode) != 0){
		sprintf(tmp, "SDL cound not get Diaply mode: %s", SDL_GetError());
		error(tmp);
	}

	sprintf(tmp, "Create Display (%d x %d)[%dHz]", mode.w, mode.h, mode.refresh_rate);
	info(tmp);

	if(1920/1080!=mode.w/mode.h){
		sprintf(tmp, "Incompatible resolution (%d x %d)!", mode.w, mode.h);
		error(tmp);
	}

	// Create a SDL window and renderer
	SDL_Window* window = NULL;
	SDL_Renderer* render = NULL;
	if(SDL_CreateWindowAndRenderer(mode.w, mode.h, SDL_WINDOW_SHOWN || SDL_WINDOW_BORDERLESS, &window, &render) < 0){
		sprintf(tmp, "SDL cond not create window! Error: %s", SDL_GetError());
		error(tmp);
	}

	// Init the window for updates
	init_update(window);

	// Load data
	loader(window);

	info("Go to main menu");

	// Enter the menu state's loop
	menu_state(window, render);

	// Cleanup
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	window = NULL;
	render = NULL;
	SDL_Quit();
	return 0;
}

bool init_systems(){
	bool success = 1;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		success = 0;
		sprintf(tmp, "SDL cound not init! Error: %s", SDL_GetError());
		error(tmp);
	}
	
	if(TTF_Init() < 0){
		success = 0;
		sprintf(tmp, "TTF could not init! Error: %s", TTF_GetError());
		error(tmp);
	}

	return success;
}

void get_display( void ){
	SDL_DisplayMode mode;
	SDL_DisplayMode curr;

	if(SDL_GetDesktopDisplayMode(0, &curr) != 0){
		sprintf(tmp, "SDL cound not get display mode: %s", SDL_GetError());
		error(tmp);
	}
	Uint32 g = curr.format;
	SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i (%iHz) [%d]", g, SDL_BITSPERPIXEL(g),SDL_GetPixelFormatName(g), curr.w, curr.h, curr.refresh_rate, curr.w/curr.h==1920/1080);

	int display_count = SDL_GetNumVideoDisplays();
	sprintf(tmp, "Found %d displays!", display_count);
	info(tmp);


	for(int i = 0; i < display_count; i++){
		int display_modes = SDL_GetNumDisplayModes(i);
			sprintf(tmp, "=====DISPLAY %d=====", i);
			info(tmp);
			for(int j = 0; j < display_modes; j++){
				if(SDL_GetDisplayMode(i, j, &mode) != 0){
					sprintf(tmp, "SDL display mode error! %s", SDL_GetError());
					error(tmp);
					exit(-1);
				}
				Uint32 f = mode.format;

				SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i (%iHz) [%d]", i, SDL_BITSPERPIXEL(f),SDL_GetPixelFormatName(f), mode.w, mode.h, mode.refresh_rate, mode.w/mode.h==1920/1080);
			}
	}
}

