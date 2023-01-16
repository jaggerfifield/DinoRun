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

enum Ids {ID_BACKGROUND, ID_PLAY, ID_STORY, ID_EXIT};
enum State {MENU, PLAY, STORY, EXIT};
enum Pos {CENTER = -1};
enum Data {JIMAGE, JFONT};
#define MENU_SIZE 3

int state;

int handle_keys(SDL_Event, int);
void update(SDL_Window*, struct Jdata*, int);

int menu_state(SDL_Window* window){
	state = MENU;
	char* path = "Assets/font.ttf";

	struct Jimage* bg = make_image(0, 0, "Assets/bg_fill.bmp");

	struct Jdata* menu = init("menu", JIMAGE, ID_BACKGROUND, bg, NULL);
	menu = add_data(menu, JFONT, ID_PLAY, CENTER, 0, path, "Play");
	menu = add_data(menu, JFONT, ID_STORY, CENTER, 100, path, "Story");
	menu = add_data(menu, JFONT, ID_EXIT, CENTER, 200, path, "Exit");

	bool quit = false;

	SDL_Event e;

	int location = 0;

	while(!quit && (state == MENU)){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
				state = EXIT;
			}else if(e.type == SDL_KEYDOWN){
				location = handle_keys(e, location);
			}
		}

		update(window, menu, location);
	}
	
	image_free(bg);

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

void update(SDL_Window* window, struct Jdata* data, int location){
	// Update the selected font
	struct Jdata* node = data;
	
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

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


