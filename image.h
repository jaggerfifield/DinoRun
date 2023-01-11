#include <SDL.h>

struct Jimage{
	char* path;
	SDL_Rect rect;
	SDL_Surface* img;
};

struct Jimage* make_image(int, int, char*);
SDL_Surface* render_image(struct Jimage*);
void free_image(struct Jimage*);
void color_key(struct Jimage*);
