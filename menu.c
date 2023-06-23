#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "jdata.h"
#include "main.h"
#include "jio.h"

#define MENU_SIZE 3

int state;

int handle_keys(SDL_Event, int);

void* load_data(void);
void update(SDL_Window*, struct Jdata**, int);

int menu_state(SDL_Window* window){
	state = MENU;

	bool quit = false;

	SDL_Event e;

	int location = 0;

	struct Jdata* DTA[4];
	DTA[0] = init(ID_MENU_BACKGROUND, JIMAGE, 0, 0, "Menu Background", "Assets/bg_fill.bmp", NULL);
	DTA[1] = init(ID_MENU_PLAY, JFONT, CENTER, 0, "Play text", "Assets/font.ttf", "Play");
	DTA[2] = init(ID_MENU_STORY, JFONT, CENTER, 100, "Stroy text", "Assets/font.ttf", "Story");
	DTA[3] = init(ID_MENU_EXIT, JFONT, CENTER, 200, "Exit text", "Assets/font.ttf", "Exit");

	info("Done loading data!");

	while(!quit && (state == MENU)){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
				state = EXIT;
			}else if(e.type == SDL_KEYDOWN){
				location = handle_keys(e, location);
			}
		}

		update(window, DTA, location);
	}

	jdata_free(DTA[0]);
	jdata_free(DTA[1]);
	jdata_free(DTA[2]);
	jdata_free(DTA[3]);

	return state;
}

int handle_keys(SDL_Event e, int location){
	int key = e.key.keysym.sym;
	if(key == SDLK_UP){
		location -= 1;

		// Check for wrap-around
		if(location < 0)
			location = 2;

	}else if(key == SDLK_DOWN){
		location += 1;

		if(location > (MENU_SIZE - 1) )
			location = 0;
	}else if(key == SDLK_RETURN){
		if(location == 0)
			state = PLAY;
		else if(location == 1)
			state = STORY;
		else if(location == 2)
			state = EXIT;
	}
	
	return location;
}

void update(SDL_Window* window, struct Jdata** data, int location){
	
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	// TODO: dynamic data length (pass size in update)
	// for(int i = 0; i < size; i++){
	
	for(int i = 0; i < 4; i++){
		struct Jdata* node = data[i];

		SDL_Rect loc;
		loc.x = node->x;
		loc.y = node->y;
		
		if(node->type == JFONT){
			if(location == node->id - 1){
				set_fgColour(node, 255, 0 , 0);
				render(node);
			}else{
				set_fgColour(node, 0, 0, 0);
				render(node);
			}
		}

		SDL_BlitSurface(node->data, NULL, win_surface, &loc);
	}

	SDL_UpdateWindowSurface(window);
}

