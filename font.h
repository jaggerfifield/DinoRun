#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

struct Jfont{
	int size;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Surface* img;
	char* text;
	const char* path;
};

struct Jfont* make_font(int, int, int, SDL_Color, const char*, char*);
SDL_Surface* render_font(struct Jfont*);
void font_free(struct Jfont*);

