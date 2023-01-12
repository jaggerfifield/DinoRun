#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

struct Jimage{
	char* path;
	SDL_Rect rect;
	SDL_Surface* img;
};

struct Jimage* make_image(int, int, char*);
SDL_Surface* render_image(struct Jimage*);
void free_image(struct Jimage*);
void color_key(struct Jimage*);
