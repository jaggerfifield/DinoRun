#ifdef WIN
#include <SDL.h>
#endif

struct Jfont{
	int size;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Surface* img;
	char* text;
	char* path;
};

struct Jfont* make_font(int, int, int, SDL_Color, char*, char*);
SDL_Surface* render_font(struct Jfont*);
void free_font(struct Jfont*);

