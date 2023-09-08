
#include "main.h"

#define MENU_SIZE 4

// Define functions
void handle_keys(SDL_Event, bool*, int*);
void* load_data(void);
void update(SDL_Window*, struct Jdata**, int*);

void menu_state(SDL_Window* window){

	// Declare variables
	SDL_Event e;
	int location = 0;
	bool selected = false;
	bool quit = false;

	// Load assets
	struct Jdata* DTA[5];
	DTA[0] = init(ID_MENU_BACKGROUND, JIMAGE, 0, 0, "Menu Background", "Assets/bg_fill.bmp", NULL);
	DTA[1] = init(ID_MENU_PLAY, JFONT, CENTER, 0, "Play text", "Assets/font.ttf", "Play");
	DTA[2] = init(ID_MENU_STORY, JFONT, CENTER, 100, "Stroy text", "Assets/font.ttf", "Story");
	DTA[3] = init(ID_MENU_SETTINGS, JFONT, CENTER, 200, "Settings text", "Assets/font.ttf", "Settings");
	DTA[4] = init(ID_MENU_EXIT, JFONT, CENTER, 300, "Exit text", "Assets/font.ttf", "Exit");

	info("Done loading data!");

	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				quit = true;
			else if(e.type == SDL_KEYDOWN)
				handle_keys(e, &selected, &location);
			if(selected){
				selected = false;
				if(location == 0)
					play_state(window);
				else if(location == 1)
					warn("WIP");
				else if(location == 2)
					info("goto settings");
				else if(location == 3)
					quit = true;
			}
		}
		update(window, DTA, &location);
	}

	jdata_free(DTA[0]);
	jdata_free(DTA[1]);
	jdata_free(DTA[2]);
	jdata_free(DTA[3]);
	jdata_free(DTA[4]);
	
	return;	
}

void handle_keys(SDL_Event e, bool* selected, int* location){
	
	int key = e.key.keysym.sym;
	
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

void update(SDL_Window* window, struct Jdata** data, int* location){
	
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	// TODO: dynamic data length (pass size in update)
	// for(int i = 0; i < size; i++){
	
	for(int i = 0; i < 5; i++){
		struct Jdata* node = data[i];

		SDL_Rect temp_rect = get_rect(node);
		
		if(node->type == JFONT){
			if(*location == node->id - 1){
				set_fgColour(node, 255, 0 , 0);
				render(node);
			}else{
				set_fgColour(node, 0, 0, 0);
				render(node);
			}
		}

		SDL_BlitSurface(node->data, NULL, win_surface, &temp_rect);
	}

	SDL_UpdateWindowSurface(window);
}

