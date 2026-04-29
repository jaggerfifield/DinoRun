#include <SDL3/SDL.h>

#include <stdbool.h>

#include "main.h"
#include "menu.h"
#include "jio.h"

// ===== settings registers =====
// ra --> Current loaction in settings
// rb --> Current page in settings
// rc --> Input event

// Define functions
static void handle_keys(SDL_KeyboardEvent, Jgame*);
void load_page(Jgame*);
static void update(Jgame*, struct Jdata**);
const char* get_display_name(Jgame*);
void restore_changes(Jgame* game_state);

enum {PAGE_1, PAGE_2, PAGE_3};
enum {IDLE, ENTER, LEFT, RIGHT};

void settings_state(Jgame* game_state){

	// Declare variables
	SDL_Event e;
    bool go_back = false;

    // Clear registers
    game_state->ra = 0;
    game_state->rb = 0;
    game_state->rc = 0;

	// Load assets
    struct Jdata** DTA = game_state->data_pack;

    // Load the strings of current page
    load_page(game_state);

	while(!game_state->quit && !go_back){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_EVENT_QUIT)
				game_state->quit = true;
			else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, game_state);
	        else if(e.type == SDL_EVENT_WINDOW_RESIZED)
                game_state = resize_window(game_state);
            else if(e.type == SDL_EVENT_MOUSE_MOTION)
                handle_mouse(e, ID_SETTINGS, game_state);
            else if(e.type == SDL_EVENT_MOUSE_BUTTON_UP)
                if(e.button.button == 1)
                    game_state->rc = 1;

            // Handle pressing/clicking
            switch(game_state->rc){
                case IDLE:
                    break;

                case ENTER:
                    switch(game_state->rb){
                        case PAGE_1:
                            switch(game_state->ra){
                                case 1:
					                info("settings.c : Current volume is %d", game_state->volume);
                            }break;

                        case PAGE_2:
                            switch(game_state->ra){
                                case 1:
                                    int win_x, win_y;
                                    SDL_Rect bounds;
                                    SDL_GetWindowPosition(game_state->window, &win_x, &win_y);
                                    SDL_GetDisplayBounds(game_state->display_id[game_state->monitor], &bounds);
					                SDL_SetWindowPosition(game_state->window, bounds.x+(bounds.w-game_state->display_w)/2, bounds.y+(bounds.h-game_state->display_h)/2);
                                    info("settings.c : Current display is [%d]", game_state->monitor);
                                    break;
                                
                                case 2:
                                    info("WIP");
                                    break;
                
                                case 3:
                                    if(!SDL_SetWindowFullscreen(game_state->window, game_state->is_fullscreen))
                                        error("settings.c : Could not set fullscreen to %d. Error: %s", game_state->is_fullscreen, SDL_GetError());
                                    break;
                                
                                case 5:
                                    if(!SDL_SetRenderVSync(game_state->renderer, ( game_state->is_vsync ) ? 1 : SDL_RENDERER_VSYNC_DISABLED))
                                        error("settings.c : Could not set vsync Error: %s", SDL_GetError());
                            }
                            break;

                        case PAGE_3:
                            break;
                    
                    }
                
                    if(game_state->ra == 6)
                        go_back = true;	

                    game_state->rc = IDLE;
                    load_page(game_state);
                    break;

                case LEFT:
                    switch(game_state->rb){
                        case PAGE_1:
                            switch(game_state->ra){
                                case 0 :
                                    // Switch pages
                                    game_state->rb = PAGE_3; // TODO if we add more pages this needs to change!
                                    break;

                                case 1:
                                    // Change Audio device
                                    break;

                                case 2:
                                    // Adjust Volume
                                    if(game_state->volume > 0)
                                        game_state->volume -= 1;
                                    if(!SDL_SetAudioStreamGain(game_state->audio_stream, (float)game_state->volume / 100.00))
                                        error("settings.c : Could not set audio stream gain [%s]", SDL_GetError());
                                    play_sound(game_state->data_pack[ID_SOUND_JUMP], game_state->audio_stream);
                                    break;

                                case 3:
                                    // Adjust music volume
                                    if(game_state->music_volume > 0)
                                        game_state->music_volume -= 1;
                                    if(!SDL_SetAudioStreamGain(game_state->music_stream, (float)game_state->music_volume / 100.00))
                                        error("settings.c : Could not set audio stream gain [%s]", SDL_GetError());
                            }
                            break;

                        case PAGE_2:
                            switch(game_state->ra){
                                case 0:
                                    // Switch pages
                                    game_state->rb = PAGE_1;
                                    break;
                                case 1:
                                    // Swap selected monitor
                                    if(game_state->monitor == 0)
                                        game_state->monitor = game_state->n_displays-1;
                                    else
                                        game_state->monitor -= 1;
                                    break;
                                
                                case 2:
                                    // Change Resolution
                                    break;

                                case 3:
                                    // Change Fullscreen
                                    game_state->is_fullscreen = !game_state->is_fullscreen;
                                    break;
                                
                                case 4:
                                    // Change FPS Limit
                                    if(game_state->fps_limit > 30)
                                        game_state->fps_limit = (int)(game_state->fps_limit / 2);
                                    break;

                                case 5:
                                    // Change VSync
                                    game_state->is_vsync = !game_state->is_vsync;
                                    break;

                            }
                            break;

                        case PAGE_3:
                            switch(game_state->ra){
                                case 0:
                                    // Switch pages
                                    game_state->rb = PAGE_2;
                                    break;
                            }
                            break;
                    }
                    game_state->rc = IDLE;
                    load_page(game_state);
                    break;

                case RIGHT:
                    switch(game_state->rb){
                        case PAGE_1:
                            switch(game_state->ra){
                                case 0:
                                    // Switch pages
                                    game_state->rb = PAGE_2;
                                    break;

                                case 1:
                                    // Change Audio Device
                                    break;

                                case 2:
                                    // Adjust volume
                                    if(game_state->volume < 200)
                                        game_state->volume += 1;
                                    if(!SDL_SetAudioStreamGain(game_state->audio_stream, (float)game_state->volume / 100.00))
                                        error("settings.c : Could not set audio stream gain [%s]", SDL_GetError());
                                    play_sound(game_state->data_pack[ID_SOUND_JUMP], game_state->audio_stream);
                                    break;

                                case 3:
                                    // Adjust music volume
                                    if(game_state->music_volume < 200)
                                        game_state->music_volume += 1;
                                    if(!SDL_SetAudioStreamGain(game_state->music_stream, (float)game_state->music_volume / 100.00))
                                        error("settings.c : Could not set audio stream gain [%s]", SDL_GetError());
                                    break;
                            }
                            break;
                        case PAGE_2:
                            switch(game_state->ra){
                                case 0:
                                    // Switch pages
                                    game_state->rb = PAGE_3;
                                    break;
                                    
                                case 1:
                                    // Change Display
                                    if(game_state->monitor >= game_state->n_displays-1)
                                        game_state->monitor = 0;
                                    else
                                        game_state->monitor += 1;
                                    break;

                                case 2:
                                    // Change Resolution
                                    break;

                                case 3:
                                    // Change Fullscreen
                                    game_state->is_fullscreen = !game_state->is_fullscreen;
                                    break;

                                case 4:
                                    // Change FPS Limit
                                    if(game_state->fps_limit < 120)
                                        game_state->fps_limit = game_state->fps_limit * 2;
                                    break;

                                case 5:
                                    // Change VSync
                                    game_state->is_vsync = !game_state->is_vsync;
                                    break;
                            }
                            break;
                        case PAGE_3:
                            switch(game_state->ra){
                                case 0:
                                    // Switch pages
                                    game_state->rb = PAGE_1;
                                    break;
                            }
                            break;
                    }
                    game_state->rc = IDLE;
                    load_page(game_state);
                    break;

            }
		}
		update(game_state, DTA);
	}

    restore_changes(game_state);

	return;	
}

void load_page(Jgame* game_state){

    switch(game_state->rb){
        
        case PAGE_1:
            set_string(game_state->data_pack[ID_SETTINGS_1], "<  Audio Device  >");
            set_string(game_state->data_pack[ID_SETTINGS_2], "<  Volume %.3d%%  >", game_state->volume);
            set_string(game_state->data_pack[ID_SETTINGS_3], "<  Music %.3d%%  >", game_state->music_volume);
            set_string(game_state->data_pack[ID_SETTINGS_4], "<    >");
            set_string(game_state->data_pack[ID_SETTINGS_5], "<    >");
            break;

        case PAGE_2:
            set_string(game_state->data_pack[ID_SETTINGS_1], "<  Display: %s  > *", get_display_name(game_state));
            set_string(game_state->data_pack[ID_SETTINGS_2], "<  Resolution %d x %d  >", game_state->display_w, game_state->display_h);
            set_string(game_state->data_pack[ID_SETTINGS_3], "<  Fullscreen %s  >", game_state->is_fullscreen ? "True" : "False");
            set_string(game_state->data_pack[ID_SETTINGS_4], "<  FPS Limit %d  >", game_state->fps_limit);
            set_string(game_state->data_pack[ID_SETTINGS_5], "<  VSync %s  >", game_state->is_vsync ? "True" : "False");
            break;

        case PAGE_3:
            set_string(game_state->data_pack[ID_SETTINGS_1], "<    >");
            set_string(game_state->data_pack[ID_SETTINGS_2], "<    >");
            set_string(game_state->data_pack[ID_SETTINGS_3], "<    >");
            set_string(game_state->data_pack[ID_SETTINGS_4], "<    >");
            set_string(game_state->data_pack[ID_SETTINGS_5], "<    >");
            break;
    
    }
}

static void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){
	
	int key = e.key;

    play_sound(game_state->data_pack[ID_SOUND_MENU], game_state->audio_stream);
	
	if(key == SDLK_UP){
		game_state->ra -= 1;
        if(game_state->ra < 0)
            game_state->ra = 6;
	}else if(key == SDLK_DOWN){
		game_state->ra += 1;
        if(game_state->ra > 6)
            game_state->ra = 0;
	}else if(key == SDLK_RIGHT)
		game_state->rc = RIGHT;
	else if(key == SDLK_LEFT)
		game_state->rc = LEFT;
	else if(key == SDLK_RETURN)
		game_state->rc = ENTER;
	
}

static void update(Jgame* game_state, struct Jdata** data){

    struct Jdata* bg = game_state->data_pack[ID_DATA_BACKGROUND];
    SDL_BlitSurface(bg->data.data, NULL, game_state->surface, bg->pram.rect);

    int i = ID_SETTINGS+1;
	
    while(data[i] != NULL){
		struct Jdata* node = data[i];
        int location = node->id-ID_SETTINGS-1;

		node->pram.rect = get_rect(node); // TODO rect is generated in render(), do we need this?

		if(node->type == JFONT){
			if(game_state->ra == location){
				set_fgColour(node, 255, 0 , 0);
			}else{
				set_fgColour(node, 0, 0, 0);
			}
		    
            render(node); // TODO : add update flag to node to check if it needs to be rendered!
            set_pos_x(node, -1, game_state);
        }

		SDL_BlitSurface(node->data.data, NULL, game_state->surface, node->pram.rect);
	    i++;
    }
    
	SDL_UpdateWindowSurface(game_state->window);
}

const char* get_display_name(Jgame* game_state){
    // Load current display
    const char* name = SDL_GetDisplayName(game_state->display_id[game_state->monitor]);
    
    if(name == NULL)
        error("settings.c : SDL could not get display name! Error: %s", SDL_GetError());
    
    return name;
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

