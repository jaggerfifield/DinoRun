
#include <stdio.h>
#include "jdata.h"
#include "image.h"
#include "font.h"
#include "main.h"

void data_test(struct Jdata* data){
	struct Jdata* node = data;

	printf("SIZE OF JDATA: %lu\n", sizeof(struct Jdata));
	printf("\tSIZE OF int: %lu\n", sizeof(int));
	printf("\tSIZE OF char*: %lu\n", sizeof(char*));
	printf("\tSIZE OF void*: %lu\n", sizeof(void*));
	printf("\tSIZE OF void*: %lu\n", sizeof(void*));
	printf("\tSIZE OF struct Jdata*: %lu\n", sizeof(struct Jdata*));
	printf("\tSIZE OF int: %lu\n", sizeof(int));
	
	printf("SIZE OF JIMAGE:%lu\n", sizeof(struct Jimage));
	printf("SIZE OF JFONT: %lu\n", sizeof(struct Jfont));
	
	while(node != NULL){
		printf("NODE ADDR: %p\n", &node);
		printf("NODE ID  : %p (%d)\n", &node->id, node->id);
		printf("NODE NAME: %p (%s)\n", &node->name, node->name);
		printf("NODE DATA: %p\n", node->data);
		printf("NODE NEXT: %p\n", node->next);
		printf("NODE TAIL: %p\n", node->tail);
		printf("NODE TYPE: %d (", node->type);
		if(node->type == JIMAGE){
			struct Jimage* temp = node->data;
			printf("JIMAGE)\n");
			printf("\tPATH: %s\n", temp->path);
			printf("\tRECT: %p\n", &temp->rect);
			printf("\t IMG: %p\n", temp->img);
		}else if(node->type == JFONT){
			struct Jfont* temp = node->data;
			printf("JFONT)\n");
			printf("\tSIZE: %d\n", temp->size);
			printf("\tRect: %p\n", &temp->rect);
			printf("\tCOLR: %p\n", &temp->color);
			printf("\tIMG : %p\n", temp->img);
			printf("\tTEXT: %s\n", temp->text);
			printf("\tPATH: %s\n", temp->path);
		}
	node = node->next;
	}
}

void font_test(struct Jfont* node){
	printf("JFONT\n");
	printf("\tSIZE: %d\n", node->size);
	printf("\tRect: %p\n", &node->rect);
	printf("\tCOLR: %p\n", &node->color);
	printf("\tIMG : %p\n", node->img);
	printf("\tTEXT: %s\n", node->text);
	printf("\tPATH: %s\n", node->path);
}

void image_test(struct Jimage* node){
	printf("JIMAGE)\n");
	printf("\tPATH: %s\n", node->path);
	printf("\tRECT: %p\n", &node->rect);
	printf("\t\tX: %ls\n", &node->rect.x);
	printf("\t\tY: %ls\n", &node->rect.y);
	printf("\t\tH: %ls\n", &node->rect.h);
	printf("\t\tW: %ls\n", &node->rect.w);
	printf("\t IMG: %p\n", node->img);

}
