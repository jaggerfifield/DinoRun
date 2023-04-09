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

struct Jdata* init(int size){
	// Reserve space for ''size'' elements (how many chuncks are we holding)
	struct Jdata* data_node = malloc(size * sizeof(struct Jdata));

	// ID must start at -1 since we increase id by one for every element (first element is -1 + 1)
	data_node->id   = -1;
	data_node->size = size;
	data_node->name = NULL;
	data_node->data = NULL;
	data_node->next = NULL;
	data_node->type = -1;

	// Set the tail to the node, since there are no other nodes.
	data_node->tail = data_node;
	
	return data_node;
}

struct Jdata* add_data(struct Jdata* node, int type, char* name, int id_, int x, int y, const char* path, char* ext){
	struct Jdata* edit = node->tail;
	int id = edit->id + 1;

	if(id_ != id)
		printf("WARN!\n");

	if(type == JIMAGE){
		//JIMAGE type
		struct Jimage* temp = make_image(x, y, path);

		// Create and fill the data node
		// TODO make this a function
		struct Jdata* new_node = (node + (id * sizeof(struct Jdata)));
		new_node->id = id;
		new_node->type = type;
		new_node->size = edit->size;
		new_node->name = name;
		new_node->data = temp;
		new_node->next = NULL;

		// Append the new node to our data
		edit->next = new_node;
		node->tail = new_node;
	}else if(type == JFONT){
		//JFONT type
		SDL_Color color = {0, 0, 0};
		struct Jfont* temp = make_font(x, y, 40, color, path, ext);

		// Create and fill the data node
		// TODO make this a function
		struct Jdata* new_node = (node + (id * sizeof(struct Jdata)));
		new_node->id = id;
		new_node->type = type;
		new_node->size = edit->size;
		new_node->name = name;
		new_node->data = temp;
		new_node->next = NULL;

		// Append new_node to our data
		edit->next = new_node;
		node->tail = new_node;
	}

	return node;
}

void* find_node(struct Jdata* data, int id){
	struct Jdata* node = data;

	// Calculate the memory location
	int location = sizeof(struct Jdata) * id;

	// Old search is slow, we know the memory size so dont need to search
	//while(node != NULL){
	//	if(node->id == id){
	//		//printf("Found %s, with id %d\n", node->name, node->id);
	//		return node->data;
	//	}
	//	node = node->next;
	//}
	printf("Attempting to find ID: %d\n", id);
	printf("WE are here: %p\n", (node + location));
	return (node + location)->data;
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

