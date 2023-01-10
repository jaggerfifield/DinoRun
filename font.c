#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct{
	int size;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Surface* img;
	char* text;
}Jfont;

bool font_init();
SDL_Surface* render_font(Jfont*);

Jfont* make_font(int x, int y, int s, SDL_Color col, char* content){
	if(!font_init())
		return NULL;

	Jfont* font = malloc(sizeof(Jfont));
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = 0;
	rect.h = 0;

	char* string;
	string = content;

	font->size = s;
	font->rect = rect;
	font->color = col;
	font->text = string;

	font->img = render_font(font);

	return font;
}

SDL_Surface* render_font(Jfont* font){
	TTF_Font* fnt = TTF_OpenFont("./arial.ttf", font->size);
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

