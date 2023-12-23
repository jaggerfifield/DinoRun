#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>

#include "main.h"
#include "jdata.h"
#include "update.h"

struct Jdata** pack[2];

void loader(SDL_Window* window){
	// Splash Screen
	struct Jdata* splash[3];
	splash[0] = init(0, JIMAGE, -1, -1, "Splash Icon", "Assets/splash.bmp", NULL, window);
	splash[1] = NULL;//init(1,);
	splash[2] = NULL;

	update(window, splash, NULL);

	// Main Menu
	struct Jdata** main_menu = (struct Jdata**)malloc(8*6);
	main_menu[0] = init(ID_MAINMENU_BACKGROUND, JIMAGE, 0, 0, "Background Image", "Assets/bg_fill.bmp", NULL, window);
	main_menu[1] = init(ID_MAINMENU_PLAY, JFONT, CENTER, 0, "Play Text", "Assets/font.ttf", "Play", window);
	main_menu[2] = init(ID_MAINMENU_ARCADE, JFONT, CENTER, 100, "Arcade Text", "Assets/font.ttf", "Arcade", window);
	main_menu[3] = init(ID_MAINMENU_SETTINGS, JFONT, CENTER, 200, "Settings Text", "Assets/font.ttf", "Settings", window);
	main_menu[4] = init(ID_MAINMENU_EXIT, JFONT, CENTER, 300, "Exit Text", "Assets/font.ttf", "Exit", window);
	main_menu[5] = NULL;

	pack[DATA_MAINMENU] = main_menu;

}

void get_data(struct Jdata*** data, int id){
	*data = pack[id];
}

void free_data(struct Jdata** data){
	int i = 0;
	while(data[i] != NULL){
		jdata_free(data[i]);
		data[i] = NULL;
		i = i + 1;
	}
	free(data);
}
