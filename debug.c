#ifdef NIX
#include <SDL2/SDL.h>
#endif

#ifdef WIN
#include <SDL.h>
#endif

#include <stdbool.h>

#include "main.h"
#include "font.h"
#include "jdata.h"
#include "data_test.h"

void debug(void*, SDL_Surface*);

void debug(void* data, SDL_Surface* win_surface){
	struct Jfont* fps = find_node(data, ID_DEBUG_FPS);

	printf("WIDTH is %d - %d\n", WINDOW_WIDTH, fps->rect.w);

	SDL_Rect pos = fps->rect;
	pos.x = WINDOW_WIDTH - fps->rect.w;

	SDL_BlitSurface(fps->img, NULL, win_surface, &pos);
}
