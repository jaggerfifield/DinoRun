#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "image.h"
#include "font.h"
#include "jdata.h"

#include "menu.h"
#include "main.h"

SDL_Window* init_window(void);
void destroy_media(SDL_Surface*);
void handler(SDL_Window*);

int main(int argc, char* argv[]){
	// Create a SDL window
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;

	window = init_window();

	// The main window loop.
	handler(window);

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

	window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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
			printf("PLAY is not ready!\n");
			state = MENU;
			//play_state(window, e);
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

