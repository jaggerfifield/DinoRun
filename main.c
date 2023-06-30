
#include "main.h"

char tmp[256];

bool init_systems();

int main(int argc, char* argv[]){
	
	// Enable color in terminal for windows
	#ifdef WIN
	colorize();
	#endif

	// Create game files/logs
	init_files();
	
	if(!init_systems())
		return 0;

	info("Start program!");

	// Create a SDL window
	SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL){
		sprintf(tmp, "SDL cond not create window! Error: %s", SDL_GetError());
		error(tmp);
	}

	// Enter the menu state's loop
	menu_state(window);

	// Cleanup
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}

bool init_systems(){
	bool success = 1;

	printf("Load SDL\n");
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		success = 0;
		printf("Error: %s\n", SDL_GetError());
		//sprintf(tmp, "SDL cound not init! Error: %s", SDL_GetError());
		//error(tmp);
	}
	
	printf("Load TTF\n");
	if(TTF_Init() < 0){
		success = 0;
		sprintf(tmp, "TTF could not init! Error: %s", TTF_GetError());
		error(tmp);
	}

	return success;
}

