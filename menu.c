#ifdef WIN
#include <SDL3/SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdbool.h>
#include <stdio.h>

#include "main.h"
#include "jdata.h"
#include "data.h"
#include "jio.h"
#include "update.h"
#include "play.h"
#include "story.h"
#include "settings.h"

// How many button in menu?
#define MENU_SIZE 4

// Define functions
void handle_keys(SDL_KeyboardEvent, bool*, int*);

void menu_state(SDL_Window* window, SDL_Renderer* render){

	// Declare variables
	SDL_Event e;
	int location = 0;
	bool selected = false;
	bool quit = false;

	// Load assets
	struct Jdata** DTA = NULL;
	get_data(&DTA, DATA_MAINMENU);

	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_EVENT_QUIT)
				quit = true;
			else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, &selected, &location);
			if(selected){
				selected = false;
				if(location == 0)
					play_state(window);
				else if(location == 1)
					story_state(window);
				else if(location == 2)
					settings_state(window);
				else if(location == 3)
					quit = true;
			}
		}
		update(window, DTA, &location);
	}
	
	free_data(DTA);

	return;	
}

void handle_keys(SDL_KeyboardEvent e, bool* selected, int* location){
	
	int key = e.key;
	
	if(key == SDLK_UP){
		*location -= 1;

		// Check for wrap-around
		if(*location < 0)
			*location = MENU_SIZE - 1;

	}else if(key == SDLK_DOWN){
		*location += 1;

		if(*location > (MENU_SIZE - 1) )
			*location = 0;
	}else if(key == SDLK_RETURN)
		*selected = true;
	
}

