#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include "main.h"
#include "jio.h"
#include "jdata.h"

SDL_Surface* win_surface = NULL;
float scale;
static int w, h;

void init_update(SDL_Window* window){
	win_surface = SDL_GetWindowSurface(window);
	SDL_GetWindowSize(window, &w, &h);
	
	if(w<0 || h<0){
		error("Bad window size!");
		exit(-1);
	}

}

void update(SDL_Window* window, struct Jdata** data, int* location){
	int i = 0;
	
	while(data[i] != NULL){
		struct Jdata* node = data[i];
		
		scale = 1.0f;

		// Menu Rendering, higlight in red
		if(location != NULL && node->type == JFONT){
			if(*location == node->id - 1)
				set_fgColour(node, 255, 0, 0);
			else
				set_fgColour(node, 0, 0, 0);
			
			render(node);
		}else
			scale = (float) MAX_WIDTH / w;

		SDL_Rect _rect = get_rect(node);
		_rect.w = _rect.w / scale;
		_rect.h = _rect.h / scale;

		SDL_BlitScaled(node->data, NULL, win_surface, &_rect);
		i = i + 1;
	}

	SDL_UpdateWindowSurface(window);
}
