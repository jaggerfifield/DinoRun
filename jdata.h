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
	// fnt - loaded font package from path
	// r - red
	// g - green
	// b - blue
	// col - combined SDL color
	char* string;
	TTF_Font* fnt;
	short int r;
	short int g;
	short int b;
	SDL_Color col;
};

struct Jdata* init(int, int, int, int, char*, char*, char*, short int, short int, short int);
void render(struct Jdata*);
void set_col(struct Jdata*, short int, short int, short int);
SDL_Rect get_rect(struct Jdata*);
void jdata_free(struct Jdata*);
