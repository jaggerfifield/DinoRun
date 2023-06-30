#include "main.h"

struct Jdata* init(int id, int type, int x, int y, char* name, char* path, char* string){
	struct Jdata* data_node = malloc(sizeof(struct Jdata));

	data_node->id = id;
	data_node->type = type;
	data_node->x = x;
	data_node->y = y;
	data_node-> name = name;
	data_node->path = path;
	data_node->string = string;

	data_node->text_bg = false;
	data_node->text_size = 40;

	if(type == JIMAGE){
		// Render the image
		data_node->data = SDL_LoadBMP(path);
		
		// Apply color key
		SDL_SetColorKey(data_node->data, SDL_TRUE, SDL_MapRGB(data_node->data->format, 0, 0, 0));

	}else if(type == JFONT){
		// Load font .ttf
		data_node->fnt = TTF_OpenFont(path, data_node->text_size);

		// Make font color black
		SDL_Color colour = {0, 0, 0};
		data_node->fgColour = colour;
		
		SDL_Color transparent = {0, 0, 0};
		data_node->bgColour = transparent;

		// Render font
		if(data_node->text_bg)
			data_node->data = TTF_RenderText_Shaded(data_node->fnt, data_node->string, data_node->fgColour, data_node->bgColour);
		else
			data_node->data = TTF_RenderText_Solid(data_node->fnt, data_node->string, data_node->fgColour);
	}

	// Apply centering
	if(x == CENTER)
		data_node->x = (WINDOW_WIDTH / 2) - (data_node->data->w / 2);
	if (y == CENTER)
		data_node->y = (WINDOW_HEIGHT / 2) - (data_node->data->h / 2);

	return data_node;
}

void render(struct Jdata* node){
	if(node->data != NULL){
		SDL_FreeSurface(node->data);
		node->data = NULL;
	}

	if(node->type == JIMAGE){
		node->data = SDL_LoadBMP(node->path);
		// TODO: animation frames here?

	}else if(node->type == JFONT){
		assert(node->fnt != NULL);
		
		if(node->text_bg)
			node->data = TTF_RenderText_Shaded(node->fnt, node->string, node->fgColour, node->bgColour);
		else
			node->data = TTF_RenderText_Solid(node->fnt, node->string, node->fgColour);
	}

	assert(node->data != NULL);
}

SDL_Rect get_rect(struct Jdata* node){
	SDL_Rect rect;
	rect.x = node->x;
	rect.y = node->y;
	assert(node->data != NULL);
	rect.h = node->data->h;
	rect.w = node->data->w;
	return rect;
}

void set_fgColour(struct Jdata* node, short int r, short int g, short int b){
	SDL_Color colour = {r, g, b};
	node->fgColour = colour;
}

void set_bgColour(struct Jdata* node, short int r, short int g, short int b){
	SDL_Color colour = {r, g, b};
	node->bgColour = colour;
}

void set_text_bg(struct Jdata* node){
	node->text_bg = !node->text_bg;
}

void set_text_size(struct Jdata* node, unsigned short int size){
	node->text_size = size;
	
	if(node->fnt != NULL)
		TTF_CloseFont(node->fnt);
	
	TTF_OpenFont(node->path, node->text_size);
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

