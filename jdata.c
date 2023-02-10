#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "jdata.h"
#include "image.h"
#include "font.h"
#include "main.h"

struct Jdata* init(char* name, int type, int id, void* data, void* next){
	struct Jdata* data_node = malloc(sizeof(struct Jdata));

	data_node->name = name;
	data_node->type = type;
	data_node->id   = id;
	data_node->data = data;
	data_node->next = next;

	// Set the tail to the node, since there are no other nodes.
	data_node->tail = data_node;

	return data_node;
}

struct Jdata* add_data(struct Jdata* node, int type, int id, int x, int y, const char* path, char* ext){
	struct Jdata* edit = node->tail;

	if(type == JIMAGE){
		//JIMAGE type
		struct Jimage* temp = make_image(x, y, path);
		struct Jdata* new_node = init(edit->name, type, id, temp, NULL);
		edit->next = new_node;
		node->tail = new_node;
	}else if(type == JFONT){
		//JFONT type
		SDL_Color color = {0, 0, 0};
		struct Jfont* temp = make_font(x, y, 40, color, path, ext);
		struct Jdata* new_node = init(edit->name, type, id, temp, NULL);
		edit->next = new_node;
		node->tail = new_node;
	}

	return node;
}

void jdata_free(struct Jdata* data){
	struct Jdata* node = data;
	while(node != NULL){
		if(node->type == JIMAGE){
			image_free(node->data);
			node->data = NULL;
		}else if(node->type == JFONT){
			font_free(node->data);
			node->data = NULL;
		}
		
		node = node->next;
	}
	
	free(data);
}

