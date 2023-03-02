
#include <stdio.h>
#include "jdata.h"
#include "image.h"
#include "font.h"
#include "main.h"

void data_test(struct Jdata* data){
	struct Jdata* node = data;

	while(node != NULL){
		printf("NODE ID  : %d\n", node->id);
		printf("NODE NAME: %s\n", node->name);
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
