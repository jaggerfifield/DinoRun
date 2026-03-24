#include <SDL3/SDL.h>

#include "main.h"
#include "jdata.h"
#include "jio.h"
#include "play.h"

int choice;

static void update(Jgame*, struct Jdata**, int);
static int handle_keys(SDL_KeyboardEvent);

void gameover_state(Jgame* game_state){
	
	struct Jdata** DTA = game_state->data_pack[2];

	choice = 0;
	int sel = 1;

	SDL_Event e;

	while(!choice){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				error("NEED TO QUIT!");
			}else if(e.type == SDL_EVENT_KEY_DOWN){
				sel = (sel + handle_keys(e.key)) % 2;
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


	SDL_Rect title_rect = get_rect(title, game_state);
	SDL_Rect play_rect = get_rect(play, game_state);
	SDL_Rect exit_rect = get_rect(exit, game_state);

    play_rect.x -= 100;
    exit_rect.x += 100;

	SDL_BlitSurface(title->data, NULL, game_state->surface, &title_rect);
	SDL_BlitSurface(play->data, NULL, game_state->surface, &play_rect);
	SDL_BlitSurface(exit->data, NULL, game_state->surface, &exit_rect);

	SDL_UpdateWindowSurface(game_state->window);
}

static int handle_keys(SDL_KeyboardEvent e){
	int key = e.key;

	if(key == SDLK_RIGHT || key == SDLK_A)
		return 1;
	else if(key == SDLK_LEFT|| key == SDLK_D)
		return -1;
	else if(key == SDLK_RETURN)
		choice = 1;
	return 0;
}
