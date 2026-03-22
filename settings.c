#include <SDL3/SDL.h>

#include <stdbool.h>

#include "main.h"
#include "jdata.h"
#include "jio.h"

#define MENU_SIZE 4

unsigned short int direction = 0;

// Define functions
static void handle_keys(SDL_KeyboardEvent, bool*, int*);
void* load_data(void);
static void update(Jgame*, struct Jdata**, int*);

void settings_state(Jgame* game_state){

	// Declare variables
	SDL_Event e;
	int location = 0;
	bool selected = false;
	bool quit = false;

    SDL_Window* window = game_state->window;

	// Load assets
	struct Jdata* DTA[5];
	DTA[0] = init(0, JIMAGE, 0, 0, "Menu Background", "Assets/bg_fill.bmp", NULL, window);
	DTA[1] = init(1, JFONT, CENTER, 0, "Volume", "Assets/font.ttf", "<  Volume ---%  >", window);
	DTA[2] = init(2, JFONT, CENTER, 100, "", "Assets/font.ttf", "Empty", window);
	DTA[3] = init(3, JFONT, CENTER, 200, "", "Assets/font.ttf", "Empty", window);
	DTA[4] = init(4, JFONT, CENTER, 300, "", "Assets/font.ttf", "Back", window);

    // Load current volume level
    sprintf(DTA[1]->string, "<  Volume %.3d%%  >", game_state->volume);

	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_EVENT_QUIT)
				quit = true;
			else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, &selected, &location);
			if(selected){
				selected = false;
				if(location == 0)
					info("test");
				else if(location == 1)
					warn("TODO");
				else if(location == 2)
					info("TODO");
				else if(location == 3)
					quit = true;
			}
		}
		update(game_state, DTA, &location);
	}

	// TODO: dynamic data length
	for(int i = 0; i < 5; i++)
		jdata_free(DTA[i]);
	
	return;	
}

static void handle_keys(SDL_KeyboardEvent e, bool* selected, int* location){
	
	int key = e.key;
	
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

static void update(Jgame* game_state, struct Jdata** data, int* location){

	// TODO: dynamic data length (pass size in update)
	// for(int i = 0; i < size; i++){
	
	for(int i = 0; i < 5; i++){
		struct Jdata* node = data[i];

		SDL_Rect temp_rect = get_rect(node, game_state);
		
		if(node->type == JFONT){
			if(*location == node->id - 1){
				if(direction == 1){
					// Left
					if(node->id == 1 && game_state->volume > 0){
						// Turn volume down
						game_state->volume -= 1;
						sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
					}
				}else if(direction == 2){
					if(node->id == 1 && game_state->volume < 100){
						game_state->volume += 1;
						sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
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

		SDL_BlitSurface(node->data, NULL, game_state->surface, &temp_rect);
	}

	SDL_UpdateWindowSurface(game_state->window);
}

