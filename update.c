#include <SDL3/SDL.h>

#include <stdlib.h>

#include "main.h"
#include "jio.h"
#include "jdata.h"

void update(Jgame* game_state, struct Jdata** data, int* location){
	int i = 0;
	
    while(data[i] != NULL){
		struct Jdata* node = data[i];
		
		float scale = 1.0f;

		// Menu Rendering, higlight in red
		if(location != NULL && node->type == JFONT){
			if(*location == node->id - 1)
				set_fgColour(node, 255, 0, 0);
			else
				set_fgColour(node, 0, 0, 0);
			
			render(node);
		}else
			scale = (float) MAX_WIDTH / game_state->display_w;

	    SDL_Rect _rect = get_rect(node, game_state);
		_rect.w = _rect.w / scale;
		_rect.h = _rect.h / scale;

		SDL_BlitSurface(node->data, NULL, game_state->surface, &_rect);
		i = i + 1;
	}

	SDL_UpdateWindowSurface(game_state->window);
}
