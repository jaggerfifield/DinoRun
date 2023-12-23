
#include "main.h"

#define MENU_SIZE 4

unsigned short int direction = 0;

// TODO we need globals.c to hold our settings
int volume = 0;

// Define functions
static void handle_keys(SDL_Event, bool*, int*);
void* load_data(void);
static void update(SDL_Window*, struct Jdata**, int*);

void settings_state(SDL_Window* window){

	// Declare variables
	SDL_Event e;
	int location = 0;
	bool selected = false;
	bool quit = false;

	// Load assets
	struct Jdata* DTA[5];
	DTA[0] = init(0, JIMAGE, 0, 0, "Menu Background", "Assets/bg_fill.bmp", NULL);
	DTA[1] = init(1, JFONT, CENTER, 0, "Volume", "Assets/font.ttf", "<  Volume ---%  >");
	DTA[2] = init(2, JFONT, CENTER, 100, "", "Assets/font.ttf", "Empty");
	DTA[3] = init(3, JFONT, CENTER, 200, "", "Assets/font.ttf", "Empty");
	DTA[4] = init(4, JFONT, CENTER, 300, "", "Assets/font.ttf", "Back");

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
					warn("TODO");
				else if(location == 2)
					info("TODO");
				else if(location == 3)
					quit = true;
			}
		}
		update(window, DTA, &location);
	}

	// TODO: dynamic data length
	for(int i = 0; i < 5; i++)
		jdata_free(DTA[i]);
	
	return;	
}

static void handle_keys(SDL_Event e, bool* selected, int* location){
	
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
	}else if(key == SDLK_RIGHT)
		direction = 2;
	else if(key == SDLK_LEFT)
		direction = 1;
	else if(key == SDLK_RETURN)
		*selected = true;
	
}

static void update(SDL_Window* window, struct Jdata** data, int* location){
	
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	// TODO: dynamic data length (pass size in update)
	// for(int i = 0; i < size; i++){
	
	for(int i = 0; i < 5; i++){
		struct Jdata* node = data[i];

		SDL_Rect temp_rect = get_rect(node);
		
		if(node->type == JFONT){
			if(*location == node->id - 1){
				char str[64];
				if(direction == 1){
					// Left
					if(node->id == 1 && volume > 0){
						// Turn volume down
						volume -= 1;
						sprintf(str, "<  Volume %.3d%%  >", volume);
						node->string = str;
					}
				}else if(direction == 2){
					if(node->id == 1 && volume < 100){
						volume += 1;
						sprintf(str, "<  Volume %.3d%%  >", volume);
						node->string = str;
					}
				}


				direction = 0;

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

