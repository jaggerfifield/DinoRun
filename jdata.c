#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "jio.h"

struct Jdata* init(int id, int type, char* name, char* path, Jgame* game_state){  
	struct Jdata* data_node = SDL_malloc(sizeof(struct Jdata));

    // All jdata have id, type, and name
	data_node->id = id;
	data_node->type = type;
	data_node-> name = name;
	data_node->path = path;

	data_node->string = NULL;

    data_node->texture = NULL;
    data_node->rect = NULL;

    // Only for JFONT
    data_node->text_bg = false;

    // Allocate rect for JIMAGE/JANIMATION/JFONT
	if(data_node->type != JSOUND){
    	data_node->rect = SDL_malloc(sizeof(SDL_Rect));
    	data_node->rect->x = 0;
    	data_node->rect->y = 0;
	}

    SDL_Surface* loaded = NULL;    
	
    if(type == JIMAGE || type == JANIMATION){
    
        // Animated JIMAGE has a directory not a .bmp
        if(type == JANIMATION){
            char* _path = NULL;
            
            data_node->n_frames = f_count(path);
            data_node->current_frame = 0;
            
            // Load all images
            data_node->textures = SDL_malloc(sizeof(SDL_Texture*)*data_node->n_frames);
            
            for(int i = 0; i < data_node->n_frames; i++){
                data_node->textures[i] = NULL;
                SDL_asprintf(&_path, "%s%04d.bmp", path, i);
                
                loaded = SDL_LoadBMP(_path);
                if(loaded == NULL)
                    error("jdata.c : Could not load animation surface [%s]", SDL_GetError());

                data_node->textures[i] = SDL_CreateTextureFromSurface(game_state->renderer, loaded);
                if(data_node->textures[i] == NULL)
                    error("jdata.c : Could not load animation textures [%s]", SDL_GetError());

                SDL_free(_path);
                SDL_DestroySurface(loaded);
                
                _path = NULL;
                loaded = NULL;
            }
            // Place the first frame into the texture slot for rendering
            data_node->texture = data_node->textures[0];
        }else{
            // Render the image
		    loaded = SDL_LoadBMP(path);
            if(loaded == NULL)
                error("jdata.c : Could not load bmp [%s]", SDL_GetError());
        }

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

        data_node->buffer = NULL;

        if(!SDL_LoadWAV(path, &spec, &data_node->buffer, &data_node->length))
            error("jdata.c : Could not load WAV data! [%s]", SDL_GetError());
    }

    if(loaded != NULL){
        data_node->texture = SDL_CreateTextureFromSurface(game_state->renderer, loaded);
        if(data_node->texture == NULL)
            error("jdata.c : Could not create texture from surface! [%s]", SDL_GetError());
        SDL_SetTextureBlendMode(data_node->texture, SDL_BLENDMODE_BLEND);
    
        data_node->rect->x = 0.0f;
        data_node->rect->y = 0.0f;
        data_node->rect->w = (float)loaded->w;
        data_node->rect->h = (float)loaded->h;
        SDL_DestroySurface(loaded);
        loaded = NULL;
    }

    debug("jdata.c : [%d] %s is done loading!", id, data_node->name);
	
    if(data_node->type == JIMAGE)
        assert(data_node->texture != NULL);

    return data_node;
}

void instance(struct Jdata** dest, struct Jdata* node, int n, Jgame* game_state){
    for(int i = 0; i < n; i++){
        dest[i] = init(-node->id, node->type, node->name, node->path, game_state);
    }
}

void render(struct Jdata* node, Jgame* game_state){
	// We call render to render the texture


    get_rect(node);

    SDL_RenderTexture(game_state->renderer, node->texture, NULL, node->rect);
}

void draw_font(struct Jdata* node, Jgame* game_state){
    // If text changes we need to draw it again
    if(node->type == JFONT && (node->string != NULL)){
        SDL_Surface* new_surface = NULL;
		
		if(node->text_bg)
			new_surface = TTF_RenderText_Shaded(node->fnt, node->string, 0, node->fgColour, node->bgColour);
		else
			new_surface = TTF_RenderText_Solid(node->fnt, node->string, 0, node->fgColour);
    
        if(new_surface == NULL)
            error("jdata.c : Could not render JFONT [%s]", SDL_GetError());

        if(node->texture != NULL){
            SDL_DestroyTexture(node->texture);
            node->texture = NULL;
        }

        node->texture = SDL_CreateTextureFromSurface(game_state->renderer, new_surface);
        if(node->texture == NULL)
            error("jdata.c : Could not render animation texture [%s]", SDL_GetError());
    }

}

void next_frame(struct Jdata* node){
    // We need to change frames!
    if(node->type == JANIMATION){
        node->current_frame = node->current_frame + 1;
        if(node->current_frame > node->n_frames-1)
            node->current_frame = 0;
        node->texture = node->textures[node->current_frame];
    }
}

void play_sound(struct Jdata* node, SDL_AudioStream* stream){
    if(node->type != JSOUND){
        error("jdata.c : You cant play a sound for a non-JSOUND type");
        return;
    }
    if(!SDL_ClearAudioStream(stream))
        error("jdata.c : Cound not clear audio stream! [%s]", SDL_GetError());

    if(!SDL_PutAudioStreamData(stream, node->buffer, node->length))
        error("jdata.c : Could not put audio buffer into audio stream [%s]", SDL_GetError());

    if(!SDL_FlushAudioStream(stream))
        error("jdata.c : Could not flush audio stream! [%s]", SDL_GetError());

}

void stop_sound(SDL_AudioStream* stream){
    if(!SDL_ClearAudioStream(stream))
        error("jdata.c : Could not clear audio stream! [%s]", SDL_GetError());
}

SDL_FRect* get_rect(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot generate rect for NULL node");
        return NULL;
    }

	node->rect->h = node->texture->h;
	node->rect->w = node->texture->w;
	
    return node->rect;
}

float get_pos_x(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->rect->x;
}

float get_pos_y(struct Jdata* node){
    if(node == NULL || node->rect == NULL){
        error("jdata.c : Cannot get position of node or rect of NULL");
        return -1; // This will be seen as a valid position for the object :(
    }

    return node->rect->y;

}

void set_position(struct Jdata* node, float x, float y, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }
 
    node->rect->x = x;
	node->rect->y = y;
 

    if(node->rect->x == CENTER)
        node->rect->x = (game_state->display_w / 2.0f) - (node->texture->w / 2.0f);
    if(node->rect->y == CENTER)
        node->rect->y = (game_state->display_h / 2.0f) - (node->texture->h / 2.0f);

}

void set_pos_x(struct Jdata* node, float x, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->rect->x = x;

    if(node->rect->x == CENTER)
        node->rect->x = (game_state->display_w / 2.0f) - (node->texture->w / 2.0f);
    
}

void set_pos_y(struct Jdata* node, float y, Jgame* game_state){
    if(node->rect == NULL){
        error("jdata.c : Node rect is NULL");
        return;
    }

    node->rect->y = y;
    
    if(node->rect->y == CENTER)
        node->rect->y = (game_state->display_h / 2.0f) - (node->texture->h / 2.0f);
    
}

void set_string(struct Jdata* node, char* str, ...){
    va_list va_args;
    va_start(va_args, str);

    if(SDL_strlen(str) > 64)
        warn("jdata.c : String is larger than 64!");

    if(node->string == NULL){
        node->string = SDL_malloc(64);
    }

	SDL_vsnprintf(node->string, 64, str, va_args);
    
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
	if(node->type != JSOUND && (node->texture != NULL)){
		SDL_DestroyTexture(node->texture);
		node->texture = NULL;
	}

	// Free loaded font .ttf
	if((node->type == JFONT) && (node->fnt != NULL)){
		TTF_CloseFont(node->fnt);
		node->fnt = NULL;
	}

    // Free the text string
    if(node->type == JFONT && node->string != NULL){
        SDL_free(node->string);
        node->string = NULL;
    }

    // Free the rect
    if(node->type != JSOUND && (node->rect != NULL)){
        SDL_free(node->rect);
        node->rect = NULL;
    }

    // Free the sound data
    if(node->type == JSOUND){
        SDL_free(node->buffer);
    }


	SDL_free(node);
}

void jdata_print(struct Jdata* node){
	debug("=====Printing Jdata node=====");
	debug("    ID: %d", node->id);
	debug("    TYPE: %d", node->type);
	debug("    X, Y: %f,%f", node->rect->x, node->rect->y);
	debug("    NAME: %s", node->name);
}

