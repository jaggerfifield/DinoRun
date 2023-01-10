#include <SDL.h>

struct Jfont{
	int size;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Surface* img;
	char* text;
};

struct Jfont* make_font(int, int, int, SDL_Color, char*);
SDL_Surface* render_font(struct Jfont*);
void free_font(struct Jfont*);

