#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "image.h"

bool image_init();
struct Jimage* make_image(int, int, char*);
SDL_Surface* load_image(struct Jimage*);
void color_key(struct Jimage*);

struct Jimage* make_image(int x, int y, char* path){
	if(!image_init())
		return NULL;

	struct Jimage* image = malloc(sizeof(struct Jimage));
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	image->path = path;
	image->img = load_image(image);

	rect.h = image->img->h;
	rect.w = image->img->w;

	image->rect = rect;

	return image;
}

void color_key(struct Jimage* image){
	//              The surface, Enabeled            the format,  color black
	SDL_SetColorKey(image->img, SDL_TRUE, SDL_MapRGB(image->img->format, 0, 0, 0));
}

SDL_Surface* load_image(struct Jimage* image){
	char path[100];

	strcpy(path, SDL_GetBasePath());
	strcat(path, image->path);
	
	return SDL_LoadBMP(path);
}

void image_free(struct Jimage* image){
	SDL_FreeSurface(image->img);
	free(image);
}

bool image_init(){
	bool success = true;
	int status = 0;
	
	if(status < 0){
		success = false;
		printf("SDL_Image could not init! Error: %s\n", SDL_GetError());
	}

	return success;
}

