#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stdio.h>

#include "main.h"
#include "data.h"
#include "jio.h"
#include "play.h"
#include "story.h"
#include "settings.h"

// ===== game_state registers =====
// ra --> Current location in menu
// rb --> Size of the menu
// rc --> did we push enter
// rd --> do update on render?

// Define functions
void handle_keys(SDL_KeyboardEvent, Jgame*);
void update(Jgame*);
void update_registers(Jgame*);
void handle_mouse(SDL_Event, int, Jgame*);

void menu_state(Jgame* game_state){

	SDL_Event e;

    // Init registers
	update_registers(game_state);

    game_state->time_tick = SDL_GetTicksNS();
    game_state->animation_tick = SDL_GetTicksNS();

    while(!game_state->quit){
        game_state->start_tick = SDL_GetTicksNS();

		while(SDL_PollEvent(&e) != 0){
			
            if(e.type == SDL_EVENT_QUIT)
				game_state->quit = true;
            else if(e.type == SDL_EVENT_WINDOW_RESIZED)
                game_state = resize_window(game_state);
            else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, game_state);
            else if(e.type == SDL_EVENT_MOUSE_MOTION)
                handle_mouse(e, ID_MAINMENU, game_state);
            else if(e.type == SDL_EVENT_MOUSE_BUTTON_UP)
                if(e.button.button == 1)
                    game_state->rc = 1;
			
            // Chck if enter was pressed
            if(game_state->rc){
				game_state->rc = false;
                switch (game_state->ra){
                    case 0:
                        // Start the game
					    play_state(game_state);
                        update_registers(game_state);
                        break;
                    case 1:
                        // enter story state
					    story_state(game_state);
                        update_registers(game_state);
				        break;
                    case 2:
                        // enter settings state
					    settings_state(game_state);
                        update_registers(game_state);
                        break;
                    case 3:
                        // quit the game
					    game_state->quit = true;
                        stop_sound(game_state->audio_stream);
			            break;
                }
		    }
        }

		update(game_state);
	}

	return;	
}

void update_registers(Jgame* game_state){
    game_state->ra = 0;
    game_state->rb = 3;
    game_state->rc = 0;
    game_state->rd = UPDATE_FRAME;
}

void update(Jgame* game_state){
    if(SDL_GetTicks() - game_state->animation_tick > ANIMATION_TIME){
        game_state->animation_tick = SDL_GetTicksNS();
        game_state->rd = ANIMATION_FRAME;
    }

    if(game_state->rd == IDLE_FRAME)
        return;

    // Always render the background
    render(game_state->data_pack[ID_DATA_BACKGROUND], game_state);
    
    int i = 1;

    struct Jdata* node = game_state->data_pack[ID_MAINMENU+i];
    
    while(node != NULL){
        if(node->type == JFONT){
            if(game_state->ra == (node->id-ID_MAINMENU-1)){
                set_fgColour(node, 255, 0, 0);
            }else{
                set_fgColour(node, 0, 0, 0);
            }
            draw_font(node, game_state);
        }

        if(node->type == JANIMATION)
            if(game_state->rd == ANIMATION_FRAME)
                next_frame(node);

        render(node, game_state);
        node = game_state->data_pack[ID_MAINMENU+(i++)];
    }
    
    SDL_RenderPresent(game_state->renderer);
    
    game_state->rd = IDLE_FRAME;
}

void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){

	int key = e.key;
	
    play_sound(game_state->data_pack[ID_SOUND_MENU], game_state->audio_stream);
    game_state->rd = UPDATE_FRAME;

	if(key == SDLK_UP){
		game_state->ra -= 1;

		// Check for wrap-around
		if(game_state->ra < 0)
			game_state->ra = game_state->rb;

	}else if(key == SDLK_DOWN){
		game_state->ra += 1;

		if(game_state->ra > game_state->rb )
			game_state->ra = 0;

	}else if(key == SDLK_RETURN)
		game_state->rc = true;
	
}

void handle_mouse(SDL_Event e, int index, Jgame* game_state){
    // TODO this is slow running every mouse motion event 
    SDL_FRect mouse;
    mouse.x = (float)e.motion.x;
    mouse.y = (float)e.motion.y;
    mouse.w = 5.0f;
    mouse.h = 5.0f;

   struct Jdata* node = game_state->data_pack[index+1];
   int i = 1;

   while(node != NULL){
        if(node->type == JFONT){
            if(check_collision(&mouse, node->rect)){
                game_state->ra = node->id-index-1;
                game_state->rd = UPDATE_FRAME;
            }
        }
   
        node = game_state->data_pack[index+1+i];
        i++;
   }

}

