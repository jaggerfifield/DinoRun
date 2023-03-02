#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>

#include "data_test.h"

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
	return state;

}

static void update(SDL_Window* window, struct Jdata* data){
	struct Jdata* node = data;

	SDL_Surface* win_surface = SDL_GetWindowSurface(window);


	if(timer >= 0){
		struct Jfont* temp = NULL;

		if(timer == 3000)
			temp = find_node(data, ID_PLAY_3);
		else if(timer == 2000)
			temp = find_node(data, ID_PLAY_2);
		else if(timer == 1000)
			temp = find_node(data, ID_PLAY_1);
		else if(timer == 0)
			temp = find_node(data, ID_PLAY_GO);

		if(temp != NULL){
			// Clear the screen first!
			struct Jimage* bg = find_node(data, ID_PLAY_BACKGROUND);
			SDL_BlitSurface(bg->img, NULL, win_surface, &bg->rect);
			
			// Blit the countdown
			SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		}
		timer = timer - 1;
	}else{
		// Here is the dino run loop (after the countdown)
		int x;
	}
	
	SDL_UpdateWindowSurface(window);
}

