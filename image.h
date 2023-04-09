#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

struct Jimage{
	int x;
	int y;
	const char* path;
	SDL_Rect rect;
	SDL_Surface* img;
};

struct Jimage* make_image(int, int, const char*);
SDL_Surface* render_image(struct Jimage*);
void free_image(struct Jimage*);
void color_key(struct Jimage*);
void image_free(struct Jimage*);
