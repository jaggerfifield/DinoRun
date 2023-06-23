#ifdef WIN
#include <SDL.h>
#include <SDL_ttf.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

// TODO: Update flags, 

struct Jdata{
	// For all types
	// id - unique number of data package
	// type - enum of data type (image, font)
	// x - the x position of top right corner
	// y - the y posotion of top right corner
	// name - human readible name
	// data - the data package, a rendered surface
	int id;
	int type;
	int x;
	int y;
	char* name;
	SDL_Surface* data;

	// For image types
	// path - path to the resource/image
	// 		note path is used for font resource file .ttf
	// frames - the number of frames used for animation
	char* path;
	int frames;

	// For font types
	// string - the font string to render
	// text_bg - true/false to render background
	// fnt - loaded font package from path
	// fgColour - foreground text colour
	// bgColour - background text colour
	char* string;
	bool text_bg;
	TTF_Font* fnt;
	SDL_Color fgColour;
	SDL_Color bgColour;
};

struct Jdata* init(int, int, int, int, char*, char*, char*);
void render(struct Jdata*);

void set_text_bg(struct Jdata*);
void set_fgColour(struct Jdata*, short int, short int, short int);
void set_bgColour(struct Jdata*, short int, short int, short int);

SDL_Rect get_rect(struct Jdata*);

void jdata_free(struct Jdata*);
