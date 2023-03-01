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

static int state;
static int timer = 3000;

static void update(SDL_Window*, struct Jdata*);

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

static void update(SDL_Window* window, struct Jdata* data){
	struct Jdata* node = data;

	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	if(timer > 0){
		struct Jfont* temp;
		if(timer == 3000){
			temp = find_node(data, ID_PLAY_3);
			SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		}

		timer = timer - 1;
	}
	

	while((node != NULL) && timer == 0){
		if(node->type == JFONT){
			struct Jfont* temp = node->data;
		}else if(node->type == JIMAGE){
			struct Jimage* temp = node->data;
		}

		node = node->next;
	}
	
	SDL_UpdateWindowSurface(window);
}

