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
#include "main.h"

#define MENU_SIZE 3

int state;

int handle_keys(SDL_Event, int, bool*);
void update(SDL_Window*, struct Jdata*, int, bool*);

int menu_state(SDL_Window* window, struct Jdata* data){
	state = MENU;

	bool quit = false;

	SDL_Event e;

	int location = 0;
	bool* update_flag = malloc(sizeof(bool*));
	*update_flag = true;

	while(!quit && (state == MENU)){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
				state = EXIT;
			}else if(e.type == SDL_KEYDOWN){
				location = handle_keys(e, location, update_flag);
			}
		}

		update(window, data, location, update_flag);
	}
	
	free(update_flag);

	return state;
}

int handle_keys(SDL_Event e, int location, bool* update_flag){
	int key = e.key.keysym.sym;
	if(key == SDLK_UP){
		*update_flag = true;
		location -= 1;

		// Check for wrap-around
		if(location < 0)
			location = 2;

	}else if(key == SDLK_DOWN){
		*update_flag = true;
		location += 1;

		if(location > (MENU_SIZE - 1) )
			location = 0;
	}else if(key == SDLK_RETURN){
		*update_flag = true;
		if(location == 0)
			state = PLAY;
		else if(location == 1)
			state = STORY;
		else if(location == 2)
			state = EXIT;
	}
	
	return location;
}

void update(SDL_Window* window, struct Jdata* data, int location, bool* update_flag){
	// Update the selected font
	struct Jdata* node = data;
	
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	if(*update_flag){
		*update_flag = false;
		while(node != NULL){
			if(node->type == JFONT){
				struct Jfont* temp = node->data;
			
				if(node->id == location + 1){
					// Set the location to red
					SDL_Color red = {255, 0, 0};
					temp->color = red;
					temp->img = render_font(node->data);
			
				}else{
					// Set the rest to black
					SDL_Color black = {0, 0, 0};
					temp->color = black;
					temp->img = render_font(node->data);
				}
			
				SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		
			}else if(node->type == JIMAGE){
				// render the bg
				struct Jimage* temp = node->data;
				SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
			}

			node = node->next;
		}

		SDL_UpdateWindowSurface(window);
	}
}


