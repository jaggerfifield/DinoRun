#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>

#include "main.h"
#include "font.h"
#include "jdata.h"
#include "image.h"

int state;

void update(SDL_Window*, struct Jdata*);

int play_state(SDL_Window* window, struct Jdata* data){
	state = PLAY;

	SDL_Event e;

	while(state == PLAY){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				state = EXIT;
			}else if(e.type == SDL_KEYDOWN){
				//handle_keys();
			}
		}

		update(window, data);
	}

}

void update(SDL_Window*, struct Jdata* data){
	struct Jdata* node = data;

	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	while(node != NULL){
		if(node->type == JFONT){
			struct Jfont* temp = node->data;
		}else if(node->type == JIMAGE){
			struct Jimage* temp = node->data;
		}

		node = node->next;
	}
	
	SDL_UpdateWindowSurface(window);
}

