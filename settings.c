#include <SDL3/SDL.h>

#include <stdbool.h>

#include "main.h"
#include "jdata.h"
#include "jio.h"

unsigned short int direction = 0;

// Define functions
static void handle_keys(SDL_KeyboardEvent, bool*, int*);
void* load_data(void);
static void update(Jgame*, struct Jdata**, int*);
void update_display(Jgame*, struct Jdata**);

void settings_state(Jgame* game_state){

	// Declare variables
	SDL_Event e;
	int location = 1;
	bool selected = false;
	bool quit = false;

	// Load assets
    struct Jdata** DTA = game_state->data_pack[3];

    // Load current volume level
    sprintf(DTA[ID_SETTINGS_VOLUME]->string, "<  Volume %.3d%%  >", game_state->volume);
    render(DTA[ID_SETTINGS_VOLUME]);
    
    // Load current display
    update_display(game_state, DTA);

    // Load current resolution
    sprintf(DTA[ID_SETTINGS_RESOLUTION]->string, "<  Resolution: %d x %d  >", game_state->display_w, game_state->display_h);
    render(DTA[ID_SETTINGS_RESOLUTION]);


	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_EVENT_QUIT)
				quit = true;
			else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, &selected, &location);
			
            // Handle pressing/clicking
            if(selected){
				selected = false;
			
                if(location == ID_SETTINGS_VOLUME)
					info("settings.c : Current volume is %d", game_state->volume);
				
                else if(location == ID_SETTINGS_DISPLAY){
                    int win_x, win_y;
                    SDL_Rect bounds;
                    SDL_GetWindowPosition(game_state->window, &win_x, &win_y);
                    SDL_GetDisplayBounds(game_state->display_id[game_state->monitor], &bounds);
					SDL_SetWindowPosition(game_state->window, bounds.x+(bounds.w-game_state->display_w)/2, bounds.y+(bounds.h-game_state->display_h)/2);
                    info("settings.c : Current display is [%d]", game_state->monitor);
                
                }else if(location == ID_SETTINGS_RESOLUTION){
                    info("WIP");
                
                }else if(location == ID_SETTINGS_THEME)
					info("TODO");
				
                else if(location == ID_SETTINGS_BACK)
					quit = true;
			}
		}
		update(game_state, DTA, &location);
	}

	return;	
}

static void handle_keys(SDL_KeyboardEvent e, bool* selected, int* location){
	
	int key = e.key;
	
	if(key == SDLK_UP){
		*location -= 1;
	}else if(key == SDLK_DOWN){
		*location += 1;
	}else if(key == SDLK_RIGHT)
		direction = 2;
	else if(key == SDLK_LEFT)
		direction = 1;
	else if(key == SDLK_RETURN)
		*selected = true;
	
}

static void update(Jgame* game_state, struct Jdata** data, int* location){

    int i = 0, j = 1;
	
    while(data[i] != NULL){
		struct Jdata* node = data[i];

		SDL_Rect temp_rect = get_rect(node, game_state);
		
		if(node->type == JFONT){
            j++;
			if(*location == node->id){
				set_fgColour(node, 255, 0 , 0);
			}else{
				set_fgColour(node, 0, 0, 0);
			}

            // Handle left right settings here
			if(direction == 1){
				// Left
				if((*location == ID_SETTINGS_VOLUME) && (*location == node->id)){
					// Turn volume down
                    if(game_state->volume > 0){
					    game_state->volume -= 1;
					    sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
                    }
				}else if((*location == ID_SETTINGS_DISPLAY) && (*location == node->id)){
                    // Swap selected monitor
                    if(game_state->monitor == 0)
                        game_state->monitor = game_state->n_displays-1;
                    else
                        game_state->monitor -= 1;
                    update_display(game_state, data);
                }
			}else if(direction == 2){
                // Right
				if((*location == ID_SETTINGS_VOLUME) && (*location == node->id)){
                    // Turn Volume up
                    if(game_state->volume < 100){
					    game_state->volume += 1;
					    sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
                    }
				}else if((*location == ID_SETTINGS_DISPLAY) && (*location == node->id)){
                    // Change displays
                    if(game_state->monitor == game_state->n_displays-1){
                        game_state->monitor = 0;
                    }else
                        game_state->monitor += 1;
                    update_display(game_state, data);
                }else if((*location == ID_SETTINGS_RESOLUTION) && (*location == node->id)){
                    // Change resolution
                    info("WIP");
                }
			}

		    render(node); // TODO : add update flag to node to check if it needs to be rendered!
        }

		SDL_BlitSurface(node->data, NULL, game_state->surface, &temp_rect);
	    i++;
    }

    direction = 0;
    
    *location = *location % j;
    if(*location == 0){
        *location = *location + 1;
    }

	SDL_UpdateWindowSurface(game_state->window);
}

void update_display(Jgame* game_state, struct Jdata** DTA){
    // Load current display
    const char* name = SDL_GetDisplayName(game_state->display_id[game_state->monitor]);
    if(name == NULL)
        error("settings.c : SDL could not get display name! Error: %s", SDL_GetError());
    else
        sprintf(DTA[ID_SETTINGS_DISPLAY]->string, "<  Display: %s  >", name);
    render(DTA[ID_SETTINGS_DISPLAY]); 
}
