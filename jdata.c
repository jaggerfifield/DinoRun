#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "jio.h"

struct Jdata* init(int id, int type, int x, int y, char* name, char* path, char* string){  
	struct Jdata* data_node = malloc(sizeof(struct Jdata));

	data_node->id = id;
	data_node->type = type;
	data_node->x = x;
	data_node->y = y;

	data_node-> name = (char*)malloc(64);
    memset(data_node->name, '\0', 64);
    sprintf(data_node->name, "%s", name);

	data_node->path = path;
	
	data_node->string = (char*)malloc(128);
    memset(data_node->string, '\0', 128);
    sprintf(data_node->string, "%s", string);
    
    assert(data_node->string[127] == '\0');

    data_node->text_bg = false;
	data_node->text_size = 40;

    data_node->frames = 0;
    data_node->current_frame = 0;

	if(type == JIMAGE || type == JANIMATION){
        // Animated JIMAGE has a directory not a .bmp
        char _path[128];
        memset(_path, '\0', 128);

        if(type == JANIMATION){
            data_node->frames = f_count(path);
            // Load the first image
            sprintf(_path, "%s%d.bmp", path, data_node->current_frame);
        }else{
            sprintf(_path, "%s", path);
        }

		// Render the image
		data_node->data = SDL_LoadBMP(_path);
		
		// Apply color key
		SDL_SetSurfaceColorKey(data_node->data, true, SDL_MapSurfaceRGB(data_node->data, 0, 0, 0));

	}else if(type == JFONT){
		// Load font .ttf
		data_node->fnt = TTF_OpenFont(path, data_node->text_size);
        assert(data_node->fnt != NULL);

		// Make font color black
		SDL_Color colour = {0, 0, 0};
		data_node->fgColour = colour;
		
		SDL_Color transparent = {0, 0, 0};
		data_node->bgColour = transparent;

		// Render font
		if(data_node->text_bg)
			data_node->data = TTF_RenderText_Shaded(data_node->fnt, data_node->string, 0, data_node->fgColour, data_node->bgColour);
		else
			data_node->data = TTF_RenderText_Solid(data_node->fnt, data_node->string, 0, data_node->fgColour);
	}

    debug("jdata.c : [%d] %s is done loading!", id, data_node->name);
	return data_node;
}

void render(struct Jdata* node){
	if(node->data != NULL){
		SDL_DestroySurface(node->data);
		node->data = NULL;
	}

	if(node->type == JIMAGE){
		node->data = SDL_LoadBMP(node->path);
	
    }else if(node->type == JANIMATION){
        char _path[128];
        memset(_path, '\0', 128);
        node->current_frame = (node->current_frame+1)%node->frames;
        sprintf(_path, "%s%d.bmp", node->path, node->current_frame);
        node->data = SDL_LoadBMP(_path);
    
    }else if(node->type == JFONT){
		assert(node->fnt != NULL);
        assert(node->string[127] == '\0');
		
		if(node->text_bg)
			node->data = TTF_RenderText_Shaded(node->fnt, node->string, 0, node->fgColour, node->bgColour);
		else
			node->data = TTF_RenderText_Solid(node->fnt, node->string, 0, node->fgColour);
    }

	assert(node->data != NULL);
}

SDL_Rect get_rect(struct Jdata* node, Jgame* game_state){
	SDL_Rect rect;
	rect.x = node->x;
	rect.y = node->y;
	
    if(rect.x == CENTER)
        rect.x = (game_state->display_w / 2) - (node->data->w / 2);
    if(rect.y == CENTER)
        rect.y = (game_state->display_h / 2) - (node->data->h / 2);

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
	
	if(node->fnt != NULL){
		TTF_CloseFont(node->fnt);
        node->fnt = NULL;
    }
	
	node->fnt = TTF_OpenFont(node->path, node->text_size);
}

void jdata_free(struct Jdata* node){

	// Free the surface
	if(node->data != NULL){
		SDL_DestroySurface(node->data);
		node->data = NULL;
	}

	// Free loaded font .ttf
	if((node->type == JFONT) && (node->fnt != NULL)){
		TTF_CloseFont(node->fnt);
		node->fnt = NULL;
	}
	
    if(node->name != NULL){
        free(node->name);
        node->name = NULL;
    }

    // Free the text string
    if(node->string != NULL){
        free(node->string);
        node->string = NULL;
    }

	free(node);
}

void jdata_print(struct Jdata* node){
	debug("=====Printing Jdata node=====");
	debug("    ID: %d", node->id);
	debug("    TYPE: %d", node->type);
	debug("    X, Y: %d,%d", node->x, node->y);
	debug("    NAME: %s", node->name);
}

