#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>

#include "data_test.h"

#include "image.h"
#include "font.h"
#include "jdata.h"

#include "play.h"
#include "menu.h"
#include "main.h"

SDL_Window* init_window(void);
void destroy_media(SDL_Surface*);
void handler(SDL_Window*);
struct Jdata* load_data(int);

int main(int argc, char* argv[]){
	// Create a SDL window
	SDL_Window* window = NULL;
	window = init_window();

	// The main loop.
	handler(window);

	// Cleanup
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}

SDL_Window* init_window(){
	// Init SDL and create the window
	SDL_Window* window = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not init! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

	window = SDL_CreateWindow("DinoRun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL){
		printf("SDL could not create window! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

struct Jdata* load_data(int target){
	const char* font = "Assets/font.ttf";

	struct Jdata* data;
	if(target == LMENU){
		struct Jimage* bg = make_image(0,0, "Assets/bg_fill.bmp");
		data = init("menu", JIMAGE, ID_MENU_BACKGROUND, bg, NULL);
		data = add_data(data, JFONT, ID_MENU_PLAY, CENTER, 0, font, "Play");
		data = add_data(data, JFONT, ID_MENU_STORY, CENTER, 100, font, "Story");
		data = add_data(data, JFONT, ID_MENU_EXIT, CENTER, 200, font, "Exit");
	} else if(target == LPLAY){
		struct Jimage* bg = make_image(0,0, "Assets/bg.bmp");
		data = init("play", JIMAGE, ID_PLAY_BACKGROUND, bg, NULL);
		data = add_data(data, JIMAGE, ID_PLAY_PLAYER, 25, 100, "Assets/dino.bmp", NULL);
		data = add_data(data, JFONT, ID_PLAY_3, CENTER, CENTER, font, "3");
		data = add_data(data, JFONT, ID_PLAY_2, CENTER, CENTER, font, "2");
		data = add_data(data, JFONT, ID_PLAY_1, CENTER, CENTER, font, "1");
		data = add_data(data, JFONT, ID_PLAY_GO, CENTER, CENTER, font, "GO!");
		data = add_data(data, JFONT, ID_PLAY_SCORE, 0, 0, font, "SCORE: 0");
		data = add_data(data, JFONT, ID_PLAY_HISCORE, 0, 0, font, "High Score: 0");
		data = add_data(data, JIMAGE, ID_PLAY_OBJECT, WINDOW_WIDTH, 0, "Assets/dino.bmp", NULL);
		data = add_data(data, JIMAGE, ID_PLAY_OBJECT2, WINDOW_WIDTH, 0, "Assets/ball.bmp", NULL);
		data = add_data(data, JIMAGE, ID_PLAY_OBJECT3, WINDOW_WIDTH, 0, "Assets/OBJ3.bmp", NULL);
		data = add_data(data, JIMAGE, ID_PLAY_OBJECT4, WINDOW_WIDTH, 0, "Assets/dino.bmp", NULL);
	}
	
	data_test(data);

	return data;
}

void handler(SDL_Window* window){

	int state = MENU;
	
	while(true){
		if(state == MENU){
			struct Jdata* menu_data = load_data(LMENU);
			state = menu_state(window, menu_data);
			jdata_free(menu_data);
			menu_data = NULL;
		}else if(state == PLAY){
			struct Jdata* play_data = load_data(LPLAY);
			state = play_state(window, play_data);
			jdata_free(play_data);
			play_data = NULL;
		}else if(state == STORY){
			printf("STORY is not finished\n");
			state = MENU;
			//story_state(window, e);
		}else if(state == EXIT){
			printf("Exit state!");
			return;
		
		}else{
			printf("Unknown state! Going to menu!\n");
			state = MENU;
		}
	}
}

