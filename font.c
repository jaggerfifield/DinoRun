#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "font.h"

//typedef struct{
//	int size;
//	SDL_Rect rect;
//	SDL_Color color;
//	SDL_Surface* img;
//	char* text;
//	char* path;
//}Jfont;

bool font_init();
SDL_Surface* render_font(struct Jfont*);

struct Jfont* make_font(int x, int y, int s, SDL_Color col, char* path, char* content){
	if(!font_init())
		return NULL;

	struct Jfont* font = malloc(sizeof(struct Jfont));
	SDL_Rect rect;

	char* string;
	string = content;

	font->size = s;
	font->color = col;
	font->text = string;
	font->path = path;

	font->img = render_font(font);

	if(x == -1)
		rect.x = (WINDOW_WIDTH / 2) - (font->img->w / 2);
	else
		rect.x = x;

	if(y == -1)
		rect.y = (WINDOW_HEIGHT / 2) - (font->img->h / 2);
	else
		rect.y = y;

	rect.w = font->img->w;
	rect.h = font->img->h;

	font->rect = rect;
	
	return font;
}

SDL_Surface* render_font(struct Jfont* font){
	TTF_Font* fnt = TTF_OpenFont(font->path, font->size);
	return TTF_RenderText_Solid(fnt, font->text, font->color);
}

bool font_init(){
	bool success = true;
	int status;

	status = TTF_Init();
	
	if(status < 0){
		success = false;
		printf("SDL_TTF could not init! ttf Error: %s\n", TTF_GetError());
	}

	return success;
}

