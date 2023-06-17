#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "jdata.h"
#include "jio.h"
#include "play.h"
#include "menu.h"
#include "main.h"

SDL_Window* init_window(void);
void destroy_media(SDL_Surface*);
void handler(SDL_Window*);
struct Jdata* load_data(int);

int main(int argc, char* argv[]){
	
	// Enable color in terminal for windows
	#ifdef WIN
	colorize();
	#endif

	init_files();

	info("Start program!");
	warn("Test!");
	error("Oh no!");
	debug("Its ok, this is just a test.");

	// Create a SDL window
	SDL_Window* window = NULL;
	window = init_window();

	if(TTF_Init() < 0){
		printf("TTF could not init!\n");
		return -1;
	}

	// The main loop.
	handler(window);

	// Cleanup
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}

SDL_Window* init_window(){
	// Init SDL and create the window
	SDL_Window* window = NULL;


	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not init! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

	window = SDL_CreateWindow("DinoRun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL){
		printf("SDL could not create window! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

void handler(SDL_Window* window){

	int state = MENU;
	
	while(true){
		if(state == MENU){
			state = menu_state(window);
		}else if(state == PLAY){
			state = play_state(window);
		}else if(state == STORY){
			printf("STORY is not finished\n");
			state = MENU;
			//story_state(window, e);
		}else if(state == EXIT){
			printf("Exit state!");
			return;
		
		}else{
			printf("Unknown state! Going to menu!\n");
			state = MENU;
		}
	}
}

