#ifdef WIN
#include <SDL.h>
#include <SDL_ttf.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "font.h"
#include "main.h"

bool font_init();
SDL_Surface* render_font(struct Jfont*);

struct Jfont* make_font(int x, int y, int s, SDL_Color col, const char* path, char* content){
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

	// Open the file here once
	font->fnt = TTF_OpenFont(path, s);

	font->img = NULL;
	font->img = render_font(font);

	if(font->img == NULL){
		printf("NULL! SDL_Error %s\n", SDL_GetError());
	}

	if(x == CENTER)
		rect.x = (WINDOW_WIDTH / 2) - (font->img->w / 2);
	else
		rect.x = x;

	if(y == CENTER)
		rect.y = (WINDOW_HEIGHT / 2) - (font->img->h / 2);
	else
		rect.y = y;
	// This is segmentaion fault
	rect.w = font->img->w;
	rect.h = font->img->h;

	font->rect = rect;
	
	return font;
}

SDL_Surface* render_font(struct Jfont* font){
	if(font->img != NULL){
		SDL_FreeSurface(font->img);
		font->img = NULL;
	}

	//TTF_Font* fnt = TTF_OpenFont(font->path, font->size);

	return TTF_RenderText_Solid(font->fnt, font->text, font->color);
}

void font_free(struct Jfont* font){
	SDL_FreeSurface(font->img);
	font->img = NULL;
	free(font);
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

