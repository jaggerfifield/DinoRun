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

SDL_Window* init_window(void);
void destroy_media(SDL_Surface*);
void handler(SDL_Window*);
void update(SDL_Window*, struct Jdata*);

enum Data {JIMAGE, JFONT};
enum State {MENU, PLAY};
enum Pos {CENTER = -1, I4 = -2, II4 = -3, III4 = -4, IV4 = -5, I3 = -6, II3 = -7, III3 = -8};

int main(int argc, char* argv[]){
	// Create a SDL window
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;

	window = init_window();

	win_surface = SDL_GetWindowSurface(window);

	// The main window loop.
	handler(window);

	// Free all resources from examples and SDL
	SDL_FreeSurface(win_surface);
	win_surface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
	return 0;
}

SDL_Window* init_window(){
	// Init SDL and create the window
	int status = SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = NULL;

	if(status < 0){
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
	
	struct Jimage* bg = make_image(0,0,"Assets/bg_fill.bmp");

	// Load the menu state
	struct Jdata* menu = init("menu", JIMAGE, 0, bg, NULL);
	menu = add_data(menu, JFONT, 1, CENTER, 0, "Assets/font.ttf", "Play");
	menu = add_data(menu, JFONT, 2, CENTER, 100, "Assets/font.ttf", "Exit");
	menu = add_data(menu, JFONT, 3, CENTER, 200, "Assets/font.ttf", "Another");
	
	// Load the run state
	struct Jdata* runner = init("runner", JIMAGE, 0, bg, NULL);
	runner = add_data(runner, JIMAGE, 1, 100, 0, "Assets/dino.bmp", "");

	bool quit = false;

	SDL_Event e;

	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				quit = true;
			else if(e.type == SDL_MOUSEBUTTONDOWN)
				printf("Mouse Down\n");
			else if(e.type == SDL_KEYDOWN){
				printf("Key Down!\n");
			}
		}

		if(state == MENU){
			int i; // Handle what the menu does?
		}

		update(window, menu);
	}
}

void update(SDL_Window* window, struct Jdata* data){
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	struct Jdata* node = data;

	while(node != NULL){
		if(node->type == JIMAGE){
			struct Jimage* temp = node->data;
			SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		}else if(node->type == JFONT){
			struct Jfont* temp = node->data;
			SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		}

		node = node->next;
	}

	SDL_UpdateWindowSurface(window);
}


