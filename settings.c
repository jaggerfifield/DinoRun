#include <SDL3/SDL.h>

#include <stdbool.h>

#include "main.h"
#include "jio.h"

// ===== settings registers =====
// ra --> Current loaction in settings
// rb --> size of the settings menu
// rc --> did we push enter
// rd --> the direction 

// Define functions
static void handle_keys(SDL_KeyboardEvent, Jgame*);
void* load_data(void);
static void update(Jgame*, struct Jdata**);
void update_display(Jgame*, struct Jdata**);
void update_fullscreen(Jgame*, struct Jdata**);
void update_vsync(Jgame*, struct Jdata**);
void restore_changes(Jgame* game_state);

void settings_state(Jgame* game_state){

	// Declare variables
	SDL_Event e;
    bool go_back = false;

    // Clear registers
    game_state->ra = 0;
    game_state->rb = 8;
    game_state->rc = 0;
    game_state->rd = 0;

	// Load assets
    struct Jdata** DTA = game_state->data_pack;

    // Load current volume level
    sprintf(DTA[ID_SETTINGS_VOLUME]->string, "<  Volume %.3d%%  >", game_state->volume);
    render(DTA[ID_SETTINGS_VOLUME]);
    
    // Load current display
    update_display(game_state, DTA);

    // Load current resolution
    sprintf(DTA[ID_SETTINGS_RESOLUTION]->string, "<  Resolution: %d x %d  >", game_state->display_w, game_state->display_h);
    render(DTA[ID_SETTINGS_RESOLUTION]);

	// Load current fullscreen state
	update_fullscreen(game_state, DTA);
    
    // Load current vsync state
    update_vsync(game_state, DTA);

    sprintf(DTA[ID_SETTINGS_FPSLIMIT]->string, "<  FPS Limit: %d  >", game_state->fps_limit);

	while(!game_state->quit && !go_back){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_EVENT_QUIT)
				game_state->quit = true;
			else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, game_state);
	        else if(e.type == SDL_EVENT_WINDOW_RESIZED)
                game_state = resize_window(game_state);

            // Handle pressing/clicking
            if(game_state->rc){
				game_state->rc = false;
			
                int location = ID_SETTINGS+game_state->ra+1;

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

                else if(location == ID_SETTINGS_FULLSCREEN){
                    if(!SDL_SetWindowFullscreen(game_state->window, game_state->is_fullscreen))
                        error("settings.c : Could not set fullscreen to %d. Error: %s", game_state->is_fullscreen, SDL_GetError());
                }else if(location == ID_SETTINGS_VSYNC){
                    if(!SDL_SetRenderVSync(game_state->renderer, ( game_state->is_vsync ) ? 1 : SDL_RENDERER_VSYNC_DISABLED)){
                        error("settings.c : Could not set vsync Error: %s", SDL_GetError());
                    }
                }else if(location == ID_SETTINGS_BACK){
				    debug("Go Back");
                    go_back = true;	
                }
            }
		}
		update(game_state, DTA);
	}

    restore_changes(game_state);

	return;	
}

static void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){
	
	int key = e.key;
	
	if(key == SDLK_UP){
		game_state->ra -= 1;
        if(game_state->ra < 0)
            game_state->ra = game_state->rb;
	}else if(key == SDLK_DOWN){
		game_state->ra += 1;
        if(game_state->ra > game_state->rb)
            game_state->ra = 0;
	}else if(key == SDLK_RIGHT)
		game_state->rd = 2;
	else if(key == SDLK_LEFT)
		game_state->rd = 1;
	else if(key == SDLK_RETURN)
		game_state->rc = true;
	
}

static void update(Jgame* game_state, struct Jdata** data){

    struct Jdata* bg = game_state->data_pack[ID_DATA_BACKGROUND];
    SDL_BlitSurface(bg->data, NULL, game_state->surface, bg->rect);

    int i = ID_SETTINGS+1;
	
    while(data[i] != NULL){
		struct Jdata* node = data[i];
        int location = node->id-ID_SETTINGS-1;

        if(node->rect == NULL)
		    node->rect = get_rect(node, game_state);

		if(node->type == JFONT){
			if(game_state->ra == location){
				set_fgColour(node, 255, 0 , 0);
			}else{
				set_fgColour(node, 0, 0, 0);
			}

            // Handle left right settings here
			if(game_state->rd == 1){
				// Left
				if((game_state->ra == location) && (node->id == ID_SETTINGS_VOLUME)){
					// Turn volume down
                    if(game_state->volume > 0){
					    game_state->volume -= 1;
					    sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
                    }
				}else if((node->id == ID_SETTINGS_DISPLAY) && (game_state->ra == location)){
                    // Swap selected monitor
                    if(game_state->monitor == 0)
                        game_state->monitor = game_state->n_displays-1;
                    else
                        game_state->monitor -= 1;
                    update_display(game_state, data);
                }else if((node->id == ID_SETTINGS_FULLSCREEN) && (game_state->ra == location)){
					game_state->is_fullscreen = !game_state->is_fullscreen;
					update_fullscreen(game_state, data);
				}else if((node->id == ID_SETTINGS_VSYNC) && (game_state->ra == location)){
                    game_state->is_vsync = !game_state->is_vsync;
                    update_vsync(game_state, data);
                }else if((node->id == ID_SETTINGS_FPSLIMIT) && (game_state->ra == location)){
                    if(game_state->fps_limit > 30){
                        sprintf(node->string, "<  FPS Limit: %d  >", game_state->fps_limit /2);
                        game_state->fps_limit = game_state->fps_limit / 2;
                    }
                }
			}else if(game_state->rd == 2){
                // Right
				if((node->id == ID_SETTINGS_VOLUME) && (game_state->ra == location)){
                    // Turn Volume up
                    if(game_state->volume < 100){
					    game_state->volume += 1;
					    sprintf(node->string, "<  Volume %.3d%%  >", game_state->volume);
                    }
				}else if((node->id == ID_SETTINGS_DISPLAY) && (game_state->ra == location)){
                    // Change displays
                    if(game_state->monitor == game_state->n_displays-1){
                        game_state->monitor = 0;
                    }else
                        game_state->monitor += 1;
                    update_display(game_state, data);
                }else if((node->id == ID_SETTINGS_RESOLUTION) && (game_state->ra == location)){
                    // Change resolution
                    info("WIP");
                }else if((node->id == ID_SETTINGS_FULLSCREEN) && (game_state->ra == location)){
					game_state->is_fullscreen = !game_state->is_fullscreen;
					update_fullscreen(game_state, data);
				}else if((node->id == ID_SETTINGS_VSYNC) && (game_state->ra == location)){
                    game_state->is_vsync = !game_state->is_vsync;
                    update_vsync(game_state, data);
                }else if((node->id == ID_SETTINGS_FPSLIMIT) && (game_state->ra == location)){
                    if(game_state->fps_limit < 120){
                        sprintf(node->string, "<  FPS Limit: %d  >", game_state->fps_limit*2);
                        game_state->fps_limit = game_state->fps_limit * 2;
                    }
                }
			}

		    render(node); // TODO : add update flag to node to check if it needs to be rendered!
        }

		SDL_BlitSurface(node->data, NULL, game_state->surface, node->rect);
	    i++;
    }

    game_state->rd = 0;
    
	SDL_UpdateWindowSurface(game_state->window);
}

void update_display(Jgame* game_state, struct Jdata** DTA){
    // Load current display
    const char* name = SDL_GetDisplayName(game_state->display_id[game_state->monitor]);
    if(name == NULL)
        error("settings.c : SDL could not get display name! Error: %s", SDL_GetError());
    else
        sprintf(DTA[ID_SETTINGS_DISPLAY]->string, "<  Display: %s  > *", name);
    render(DTA[ID_SETTINGS_DISPLAY]); 
}

void update_fullscreen(Jgame* game_state, struct Jdata** DTA){
	if(game_state->is_fullscreen)
		memcpy(DTA[ID_SETTINGS_FULLSCREEN]->string, "<  Fullscreen: True  > *\0", 25);
	else
		memcpy(DTA[ID_SETTINGS_FULLSCREEN]->string, "<  Fullscreen: False  > *\0", 26);
	render(DTA[ID_SETTINGS_FULLSCREEN]);
	
}

void update_vsync(Jgame* game_state, struct Jdata** DTA){
    if(game_state->is_vsync)
        memcpy(DTA[ID_SETTINGS_VSYNC]->string, "<  VSync: True  > *\0", 20);
    else
        memcpy(DTA[ID_SETTINGS_VSYNC]->string, "<  VSync: False  > *\0", 21);
    render(DTA[ID_SETTINGS_VSYNC]);
}

void restore_changes(Jgame* game_state){
 
    if((SDL_GetWindowFlags(game_state->window) & SDL_WINDOW_FULLSCREEN) == !game_state->is_fullscreen){
        game_state->is_fullscreen = !game_state->is_fullscreen;
        info("settings.c : Rest fullscreen to previous value, not applied");
    }


    int disp = SDL_GetDisplayForWindow(game_state->window)-1;
    if(disp != game_state->monitor){
        game_state->monitor = disp;
        info("settings.c : Reset window to previous value, not applied");
    }

    int vsync;
    if(!SDL_GetRenderVSync(game_state->renderer, &vsync))
        error("settings.c : could not get vsync status. Error: %s", SDL_GetError());
    if(vsync != game_state->is_vsync){
        game_state->is_vsync = vsync;
        info("settings.c : Vsync setting not applied reverting back");
    }
    
}

