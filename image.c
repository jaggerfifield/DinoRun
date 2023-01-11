#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
	char* path;
	SDL_Rect rect;
	SDL_Surface* img;
}Jimage;

bool image_init();
Jimage* make_image(int, int, char*);
SDL_Surface* load_image(Jimage*);
void color_key(Jimage*);

Jimage* make_image(int x, int y, char* path){
	if(!image_init())
		return NULL;

	Jimage* image = malloc(sizeof(Jimage));
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

void color_key(Jimage* image){
	//              The surface, Enabeled            the format,  color black
	SDL_SetColorKey(image->img, SDL_TRUE, SDL_MapRGB(image->img->format, 0, 0, 0));
}

SDL_Surface* load_image(Jimage* image){
	return SDL_LoadBMP(strcat(SDL_GetBasePath(), image->path));
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

