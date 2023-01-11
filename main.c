#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "font.h"
#include "image.h"

#define WINDOW_WIDTH 1920/2
#define WINDOW_HEIGHT 1080/2

SDL_Window* init_window(void);
void destroy_media(SDL_Surface*);
void handler(SDL_Window*, struct Jimage**);
void update(SDL_Window*, struct Jimage**);

void load_assets(struct Jimage**, char**, size_t);

int main(int argc, char* argv[]){
	// Create a SDL window
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;

	window = init_window();

	win_surface = SDL_GetWindowSurface(window);

	// The asset array, maximum of 30 assets allowed.
	struct Jimage* assets[30];
	char* level_package[] = {"Assets/bg_fill.bmp", "Assets/dino.bmp"};

	load_assets(assets, level_package, sizeof(level_package)/sizeof(level_package[0]));
	// TODO Remember to free these later, you are not freeing them now!
	assets[1]->rect.x = 100;
	color_key(assets[1]);

	//EXAMPLE OF JDATA
	//SDL_Color S_color = {94, 129, 172};

	//struct Jfont* greeting = make_font(500, 500, 40, S_color, "This is a test");
	//SDL_BlitSurface(greeting->img, NULL, win_surface, &greeting->rect);

	//struct Jimage* block = make_image(10, 10, "Assets/Block.bmp");
	//SDL_BlitSurface(block->img, NULL, win_surface, &block->rect);
	// End of examples


	// The main window loop.
	handler(window, assets);

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

void load_assets(struct Jimage** assets, char** level_package, size_t size){
	printf("The size is %llu\n", size);
	for(int i = 0; i < (int)size; i++){
		printf("Loading asset: %s\n", level_package[i]);
		struct Jimage* temp = make_image(0, 0, level_package[i]);
		assets[i] = temp;
	}

}

void update(SDL_Window* window, struct Jimage** assets){

	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	// Gravity for character
	int gravity = -2;

	int y = WINDOW_HEIGHT - assets[1]->rect.h;

	if(assets[1]->rect.y < y)
		assets[1]->rect.y = assets[1]->rect.y - gravity;

	for(int i = 0; i < 2; i++){
		SDL_BlitSurface(assets[i]->img, NULL, win_surface, &assets[i]->rect);
	}

	SDL_UpdateWindowSurface(window);

	return;
}

void handler(SDL_Window* window, struct Jimage** assets){
	bool quit = false;
	SDL_Event e;
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				quit = true;
			else if(e.type == SDL_MOUSEBUTTONDOWN)
				printf("Mouse Down\n");
			else if(e.type == SDL_KEYDOWN){
				//key_down(e);
				printf("Key Down!\n");
				assets[1]->rect.y = assets[1]->rect.y - 500;
			}
		}
		update(window, assets);
	}

}


void key_down(SDL_Event event){
	printf("CODE: %d, W: %d\n", event.key.keysym.sym, SDLK_w);
	if(event.key.keysym.sym == SDLK_w){
		return;
	}
}




