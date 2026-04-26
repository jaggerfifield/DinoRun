#include <SDL3/SDL.h>

#include "main.h"
#include "jio.h"
#include "play.h"

int choice;

static void update(Jgame*, struct Jdata**, int);
static int handle_keys(SDL_KeyboardEvent, Jgame*);

void gameover_state(Jgame* game_state){
	
	struct Jdata** DTA = game_state->data_pack;

	choice = 0;
	int sel = 1;

	SDL_Event e;

    set_position(DTA[ID_GAMEOVER_PLAY], -1, -1, game_state);
    set_position(DTA[ID_GAMEOVER_EXIT], -1, -1, game_state);
    set_pos_x(DTA[ID_GAMEOVER_PLAY], get_pos_x(DTA[ID_GAMEOVER_PLAY])-100, game_state);
    set_pos_x(DTA[ID_GAMEOVER_EXIT], get_pos_x(DTA[ID_GAMEOVER_EXIT])+100, game_state);

	while(!choice){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				error("NEED TO QUIT!");
			}else if(e.type == SDL_EVENT_WINDOW_RESIZED){
                game_state = resize_window(game_state);
            }else if(e.type == SDL_EVENT_KEY_DOWN){
				sel = (sel + handle_keys(e.key, game_state)) % 2;
			}
		}

		update(game_state, DTA, sel);
	}
	
	if(sel)
		play_state(game_state);
}

static void update(Jgame* game_state, struct Jdata** DTA, int sel){
	
	struct Jdata* title = DTA[ID_GAMEOVER_TEXT];
	struct Jdata* play = DTA[ID_GAMEOVER_PLAY];
	struct Jdata* exit = DTA[ID_GAMEOVER_EXIT];
		
	if(sel){
		set_fgColour(play, 255, 0, 0);
		render(play);
		set_fgColour(exit, 0, 0, 0);
		render(exit);
	}else{
		set_fgColour(play, 0, 0, 0);
		render(play);
		set_fgColour(exit, 255, 0, 0);
		render(exit);
	}


    if(title->pram.rect == NULL)
	    title->pram.rect = get_rect(title);
	if(play->pram.rect == NULL){
        play->pram.rect = get_rect(play);
        play->pram.rect->x -= 100;
    }if(exit->pram.rect == NULL){
        exit->pram.rect = get_rect(exit);
        exit->pram.rect->x += 100;
    }

	SDL_BlitSurface(title->data.data, NULL, game_state->surface, title->pram.rect);
	SDL_BlitSurface(play->data.data, NULL, game_state->surface, play->pram.rect);
	SDL_BlitSurface(exit->data.data, NULL, game_state->surface, exit->pram.rect);

	SDL_UpdateWindowSurface(game_state->window);
}

static int handle_keys(SDL_KeyboardEvent e, Jgame* game_state){
	int key = e.key;

    play_sound(game_state->data_pack[ID_SOUND_MENU], game_state->audio_stream);

	if(key == SDLK_RIGHT || key == SDLK_A)
		return 1;
	else if(key == SDLK_LEFT|| key == SDLK_D)
		return -1;
	else if(key == SDLK_RETURN)
		choice = 1;
	return 0;
}
