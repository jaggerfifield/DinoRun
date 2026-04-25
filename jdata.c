#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "jio.h"

struct Jdata* init(int id, int type, char* name, char* path){  
	struct Jdata* data_node = SDL_malloc(sizeof(struct Jdata));

    // All jdata have id, type, and name
	data_node->id = id;
	data_node->type = type;
	data_node-> name = name;
	data_node->path = path;

	data_node->aux.string = NULL;
    data_node->data = NULL;

    // Only for JFONT
    data_node->text_bg = false;

    // Allocate rect for JIMAGE/JANIMATION/JFONT
    data_node->rect = SDL_malloc(sizeof(SDL_Rect));
    data_node->rect->x = 0;
    data_node->rect->y = 0;

	if(type == JIMAGE || type == JANIMATION){
        // Animated JIMAGE has a directory not a .bmp
        char _path[128];
        memset(_path, '\0', 128);

        if(type == JANIMATION){
            data_node->aux.frames.x = f_count(path);
            data_node->aux.frames.y = 0;
            // Load the first image
            sprintf(_path, "%s%04d.bmp", path, data_node->aux.frames.y);
        }else{
            sprintf(_path, "%s", path);
        }

		// Render the image
		data_node->data = SDL_LoadBMP(_path);
        if(data_node->data == NULL)
            error("jdata.c : Could not load bmp Error: %s", SDL_GetError());

		// Apply color key
		SDL_SetSurfaceColorKey(data_node->data, true, SDL_MapSurfaceRGB(data_node->data, 0, 0, 0));

	}else if(type == JFONT){
		// Load font .ttf
        data_node->fnt = NULL;
        set_font_size(data_node, 40);

		// Make font color black
		SDL_Color colour = {0, 0, 0};
		data_node->fgColour = colour;
		
		SDL_Color transparent = {0, 0, 0};
		data_node->bgColour = transparent;
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
		SDL_SetSurfaceColorKey(node->data, true, SDL_MapSurfaceRGB(node->data, 0, 0, 0));
	
    }else if(node->type == JANIMATION){
        char _path[128];
        memset(_path, '\0', 128);
        node->aux.frames.y = (node->aux.frames.y+1)%node->aux.frames.x;
        sprintf(_path, "%s%04d.bmp", node->path, node->aux.frames.y);
        node->data = SDL_LoadBMP(_path);
		SDL_SetSurfaceColorKey(node->data, true, SDL_MapSurfaceRGB(node->data, 0, 0, 0));
    
    }else if(node->type == JFONT){
		assert(node->fnt != NULL);
		
		if(node->text_bg)
			node->data = TTF_RenderText_Shaded(node->fnt, node->aux.string, 0, node->fgColour, node->bgColour);
		else
			node->data = TTF_RenderText_Solid(node->fnt, node->aux.string, 0, node->fgColour);
    }

    get_rect(node);

	assert(node->data != NULL);
}

SDL_Rect* get_rect(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot generate rect for NULL node");
        return NULL;
    }

	node->rect->h = node->data->h;
	node->rect->w = node->data->w;
	
    return node->rect;
}

int get_pos_x(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->rect->x;
}

int get_pos_y(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->rect->y;

}

void set_position(struct Jdata* node, int x, int y, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }
 
    node->rect->x = x;
	node->rect->y = y;
    
    if(node->rect->x == CENTER)
        node->rect->x = (game_state->display_w / 2) - (node->data->w / 2);
    if(node->rect->y == CENTER)
        node->rect->y = (game_state->display_h / 2) - (node->data->h / 2);

}

void set_pos_x(struct Jdata* node, int x, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->rect->x = x;

    if(node->rect->x == CENTER)
        node->rect->x = (game_state->display_w / 2) - (node->data->w / 2);
}

void set_pos_y(struct Jdata* node, int y, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->rect->y = y;
    
    if(node->rect->y == CENTER)
        node->rect->y = (game_state->display_h / 2) - (node->data->h / 2);

}

void set_string(struct Jdata* node, char* str, ...){
    va_list va_args;
    va_start(va_args, str);

    if(SDL_strlen(str) > 64)
        warn("jdata.c : String is larger than 64!");

    char text[64];
    SDL_vsnprintf(text, 63, str, va_args);
    size_t max_l = SDL_strlen(text) + 1;

    if(node->aux.string == NULL)
        node->aux.string = SDL_strdup(text);
    else
        SDL_vsnprintf(node->aux.string, max_l, str, va_args);
    
    va_end(va_args);
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

void set_font_size(struct Jdata* node, int size){
	if(node-> type != JFONT){
        warn("jdata.c : Dont set font size on a non-font node!");
        return;
    }

	if(node->fnt != NULL){
		TTF_CloseFont(node->fnt);
        node->fnt = NULL;
    }
	
	node->fnt = TTF_OpenFont(node->path, size);
    
    if(node->fnt == NULL)
        error("jdata.c : Could not set font size, %s", SDL_GetError());
}

void jdata_free(struct Jdata* node){
    if(node == NULL){
        return;
    }

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

    // Free the text string
    if(node->type == JFONT && node->aux.string != NULL){
        SDL_free(node->aux.string);
        node->aux.string = NULL;
    }

    // Free the rect
    if(node->rect != NULL){
        SDL_free(node->rect);
        node->rect = NULL;
    }

	SDL_free(node);
}

void jdata_print(struct Jdata* node){
	debug("=====Printing Jdata node=====");
	debug("    ID: %d", node->id);
	debug("    TYPE: %d", node->type);
	debug("    X, Y: %d,%d", node->rect->x, node->rect->y);
	debug("    NAME: %s", node->name);
}

