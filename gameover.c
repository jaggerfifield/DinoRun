#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include "main.h"
#include "jdata.h"
#include "jio.h"
#include "play.h"

int choice;

static void update(SDL_Window*, struct Jdata**, int);
static int handle_keys(SDL_Event);

void gameover_state(SDL_Window* window){
	
	int dSize = 3;
	struct Jdata* DTA[dSize];
	DTA[0] = init(ID_GAMEOVER_TEXT, JFONT, -1, 100, "Game Over! text", "Assets/font.ttf", "Game Over!", window);
	DTA[1] = init(ID_GAMEOVER_PLAY, JFONT, -1, -1, "Play again text", "Assets/font.ttf", "Retry", window);
	DTA[2] = init(ID_GAMEOVER_EXIT, JFONT, -1, -1, "Quit text", "Assets/font.ttf", "Back", window);
	
	struct Jdata* play = DTA[ID_GAMEOVER_PLAY];
	struct Jdata* exit = DTA[ID_GAMEOVER_EXIT];

	play->x = play->x - 100;
	exit->x = exit->x + 100;

	choice = 0;
	int sel = 1;

	SDL_Event e;

	while(!choice){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				error("NEED TO QUIT!");
			}else if(e.type == SDL_KEYDOWN){
				sel = (sel + handle_keys(e)) % 2;
			}
		}

		update(window, DTA, sel);
	}
	
	// Clean up memory
	for(int i = 0; i < dSize; i++){
		jdata_free(DTA[i]);
	}

	if(sel)
		play_state(window);
}

static void update(SDL_Window* window, struct Jdata** DTA, int sel){
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);
	
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


	SDL_Rect title_rect = get_rect(title);
	SDL_Rect play_rect = get_rect(play);
	SDL_Rect exit_rect = get_rect(exit);

	SDL_BlitSurface(title->data, NULL, win_surface, &title_rect);
	SDL_BlitSurface(play->data, NULL, win_surface, &play_rect);
	SDL_BlitSurface(exit->data, NULL, win_surface, &exit_rect);

	SDL_UpdateWindowSurface(window);
}

static int handle_keys(SDL_Event e){
	int key = e.key.keysym.sym;

	if(key == SDLK_RIGHT || key == SDLK_a)
		return 1;
	else if(key == SDLK_LEFT|| key == SDLK_d)
		return -1;
	else if(key == SDLK_RETURN)
		choice = 1;
	return 0;
}
