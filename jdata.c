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
    data_node->data.data = NULL;

    // Only for JFONT
    data_node->text_bg = false;

    // Allocate rect for JIMAGE/JANIMATION/JFONT
	if(data_node->type != JSOUND){
    	data_node->pram.rect = SDL_malloc(sizeof(SDL_Rect));
    	data_node->pram.rect->x = 0;
    	data_node->pram.rect->y = 0;
	}

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
		data_node->data.data = SDL_LoadBMP(_path);
        if(data_node->data.data == NULL)
            error("jdata.c : Could not load bmp Error: %s", SDL_GetError());

		// Apply color key
		SDL_SetSurfaceColorKey(data_node->data.data, true, SDL_MapSurfaceRGB(data_node->data.data, 0, 0, 0));

	}else if(type == JFONT){
		// Load font .ttf
        data_node->fnt = NULL;
        set_font_size(data_node, 40);

		// Make font color black
		SDL_Color colour = {0, 0, 0};
		data_node->fgColour = colour;
		
		SDL_Color transparent = {0, 0, 0};
		data_node->bgColour = transparent;
	}else if(type == JSOUND){
        SDL_AudioSpec spec;

        spec.format = SDL_AUDIO_S16LE;
        spec.channels = 2;
        spec.freq = 44100;

        data_node->data.buffer = NULL;

        if(!SDL_LoadWAV(path, &spec, &data_node->data.buffer, &data_node->pram.length))
            error("jdata.c : Could not load WAV data! [%s]", SDL_GetError());
    }

    debug("jdata.c : [%d] %s is done loading!", id, data_node->name);
	
    return data_node;
}

void render(struct Jdata* node){
	if(node->data.data != NULL){
		SDL_DestroySurface(node->data.data);
		node->data.data = NULL;
	}

	if(node->type == JIMAGE){
		node->data.data = SDL_LoadBMP(node->path);
		SDL_SetSurfaceColorKey(node->data.data, true, SDL_MapSurfaceRGB(node->data.data, 0, 0, 0));
	
    }else if(node->type == JANIMATION){
        char _path[128];
        memset(_path, '\0', 128);
        node->aux.frames.y = (node->aux.frames.y+1)%node->aux.frames.x;
        sprintf(_path, "%s%04d.bmp", node->path, node->aux.frames.y);
        node->data.data = SDL_LoadBMP(_path);
		SDL_SetSurfaceColorKey(node->data.data, true, SDL_MapSurfaceRGB(node->data.data, 0, 0, 0));
    
    }else if(node->type == JFONT){
		assert(node->fnt != NULL);
		
		if(node->text_bg)
			node->data.data = TTF_RenderText_Shaded(node->fnt, node->aux.string, 0, node->fgColour, node->bgColour);
		else
			node->data.data = TTF_RenderText_Solid(node->fnt, node->aux.string, 0, node->fgColour);
    }

    get_rect(node);

	assert(node->data.data != NULL);
}

void play_sound(struct Jdata* node, SDL_AudioStream* stream){
    if(node->type != JSOUND){
        error("jdata.c : You cant play a sound for a non-JSOUND type");
        return;
    }
    if(!SDL_ClearAudioStream(stream))
        error("jdata.c : Cound not clear audio stream! [%s]", SDL_GetError());

    if(!SDL_PutAudioStreamData(stream, node->data.buffer, node->pram.length))
        error("jdata.c : Could not put audio buffer into audio stream [%s]", SDL_GetError());

    if(!SDL_FlushAudioStream(stream))
        error("jdata.c : Could not flush audio stream! [%s]", SDL_GetError());

}

void stop_sound(SDL_AudioStream* stream){
    if(!SDL_ClearAudioStream(stream))
        error("jdata.c : Could not clear audio stream! [%s]", SDL_GetError());
}

SDL_Rect* get_rect(struct Jdata* node){
    if(node == NULL || node->pram.rect == NULL){
        error("jdata.c : Cannot generate rect for NULL node");
        return NULL;
    }

	node->pram.rect->h = node->data.data->h;
	node->pram.rect->w = node->data.data->w;
	
    return node->pram.rect;
}

int get_pos_x(struct Jdata* node){
    if(node == NULL || node->pram.rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->pram.rect->x;
}

int get_pos_y(struct Jdata* node){
    if(node == NULL || node->pram.rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->pram.rect->y;

}

SDL_Surface* get_data(struct Jdata* node){
    if(node->data.data == NULL)
        error("jdata.c : Return null data!");

    return node->data.data;
}

SDL_Rect* get_rekt(struct Jdata* node){
    return node->pram.rect;
}

void set_position(struct Jdata* node, int x, int y, Jgame* game_state){
    if(node->pram.rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }
 
    node->pram.rect->x = x;
	node->pram.rect->y = y;
    
    if(node->pram.rect->x == CENTER)
        node->pram.rect->x = (game_state->display_w / 2) - (node->data.data->w / 2);
    if(node->pram.rect->y == CENTER)
        node->pram.rect->y = (game_state->display_h / 2) - (node->data.data->h / 2);

}

void set_pos_x(struct Jdata* node, int x, Jgame* game_state){
    if(node->pram.rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->pram.rect->x = x;

    if(node->pram.rect->x == CENTER)
        node->pram.rect->x = (game_state->display_w / 2) - (node->data.data->w / 2);
}

void set_pos_y(struct Jdata* node, int y, Jgame* game_state){
    if(node->pram.rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->pram.rect->y = y;
    
    if(node->pram.rect->y == CENTER)
        node->pram.rect->y = (game_state->display_h / 2) - (node->data.data->h / 2);

}

void set_string(struct Jdata* node, char* str, ...){
    va_list va_args;
    va_start(va_args, str);

    if(SDL_strlen(str) > 64)
        warn("jdata.c : String is larger than 64!");

    if(node->aux.string == NULL)
        node->aux.string = SDL_malloc(64);

	SDL_vsnprintf(node->aux.string, 64, str, va_args);
    
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
	if(node->type != JSOUND && (node->data.data != NULL)){
		SDL_DestroySurface(node->data.data);
		node->data.data = NULL;
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
    if(node->type != JSOUND && (node->pram.rect != NULL)){
        SDL_free(node->pram.rect);
        node->pram.rect = NULL;
    }

    // Free the sound data
    if(node->type == JSOUND){
        SDL_free(node->data.buffer);
    }


	SDL_free(node);
}

void jdata_print(struct Jdata* node){
	debug("=====Printing Jdata node=====");
	debug("    ID: %d", node->id);
	debug("    TYPE: %d", node->type);
	debug("    X, Y: %d,%d", node->pram.rect->x, node->pram.rect->y);
	debug("    NAME: %s", node->name);
}

