#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "jdata.h"
#include "main.h"

void set_col(struct Jdata*, short int, short int, short int);

struct Jdata* init(int id, int type, int x, int y, char* name, char* path, char* string, short int r, short int g, short int b){
	struct Jdata* data_node = malloc(sizeof(struct Jdata));

	data_node->id = id;
	data_node->type = type;
	data_node->x = x;
	data_node->y = y;
	data_node-> name = name;
	data_node->path = path;
	data_node->string = string;
	data_node->r = r;
	data_node->g = g;
	data_node->b = b;

	if(type == JIMAGE){
		// Render the image
		data_node->data = SDL_LoadBMP(path);

		// Apply color key
		SDL_SetColorKey(data_node->data, SDL_TRUE, SDL_MapRGB(data_node->data->format, 0, 0, 0));
	}else if(type == JFONT){
		// Load font .ttf
		data_node->fnt = TTF_OpenFont(path, 40); // TODO: FONT size

		// Make font color
		SDL_Color col = {r, g, b};
		data_node->col = col;

		// Render font
		data_node->data = TTF_RenderText_Solid(data_node->fnt, data_node->string, data_node->col);
	}

	// Apply centering
	if(x == CENTER)
		data_node->x = (WINDOW_WIDTH / 2) - (data_node->data->w / 2);
	if (y == CENTER)
		data_node->y = (WINDOW_HEIGHT / 2) - (data_node->data->h / 2);

	return data_node;
}

void render(struct Jdata* node){
	if(node->type == JIMAGE){
	node->data = SDL_LoadBMP(node->path);
	// TODO: animation frames here?

	}else if(node->type == JFONT){
		assert(node->fnt != NULL);

		node->data = TTF_RenderText_Solid(node->fnt, node->string, node->col);
	}
}

void set_col(struct Jdata* node, short int r, short int g, short int b){
	node->r = r;
	node->g = g;
	node->b = b;
	SDL_Color col = {r, g, b};
	node->col = col;
}

void jdata_free(struct Jdata* node){

	// Free the surface
	if(node->data != NULL){
		SDL_FreeSurface(node->data);
		node->data = NULL;
	}

	// Free loaded font .ttf
	if(node->type == JFONT){
		TTF_CloseFont(node->fnt);
		node->fnt = NULL;
	}
	
	free(node);
}

