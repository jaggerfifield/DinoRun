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

// Define functions
void handle_keys(SDL_KeyboardEvent, Jgame*);
void update(Jgame*);
void update_registers(Jgame*);

void menu_state(Jgame* game_state){

	SDL_Event e;

    // Init registers
	update_registers(game_state);

    while(!game_state->quit){
		while(SDL_PollEvent(&e) != 0){
			
            if(e.type == SDL_EVENT_QUIT)
				game_state->quit = true;
            else if(e.type == SDL_EVENT_WINDOW_RESIZED)
                game_state = resize_window(game_state);
            else if(e.type == SDL_EVENT_KEY_DOWN)
				handle_keys(e.key, game_state);
			
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
}

void update(Jgame* game_state){
 
    struct Jdata* background = game_state->data_pack[ID_DATA_BACKGROUND];
    
    if(background->pram.rect == NULL)
        background->pram.rect = get_rect(background);

    SDL_BlitSurface(background->data.data, NULL, game_state->surface, background->pram.rect);
    
    int i = 1;

    struct Jdata* node = game_state->data_pack[ID_MAINMENU+i];
    
    while(node != NULL){
        if(game_state->ra == (node->id-ID_MAINMENU-1)){
            set_fgColour(node, 255, 0, 0); // TODO We are setting this every frame
            render(node);
        }else{
            set_fgColour(node, 0, 0, 0); // TODO We are setting this evey frame
            render(node);
        }

        SDL_BlitSurface(node->data.data, NULL, game_state->surface, node->pram.rect);
        
        node = game_state->data_pack[ID_MAINMENU+(i++)];
    }

    SDL_UpdateWindowSurface(game_state->window);
}

void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){

	int key = e.key;
	
    play_sound(game_state->data_pack[ID_SOUND_MENU], game_state->audio_stream);

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

