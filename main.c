#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "jio.h"
#include "jdata.h"
#include "data.h"
#include "menu.h"
#include "update.h"

void software_window(Jgame*);
void cleanup(Jgame* game_state);

int main(int argc, char* argv[]){

    // Enable color in terminal for windows
    #ifdef WIN
    colorize();
    #endif

    // Create game files/logs
    init_files();
 
    // Init SDL systems
    if(!SDL_Init(SDL_INIT_VIDEO)){
    	error("main.c : SDL cound not init! Error: %s", SDL_GetError());
        return 0;
    }
    if(!TTF_Init()){
    	error("main.c: TTF could not init! Error: %s", SDL_GetError());
        return 0;
    }

    info("main.c : Start program!");
 
    Jgame* game_state = (Jgame*)malloc(sizeof(Jgame));
	game_state->data_pack = NULL;

    game_state->monitor = 0; // TODO we need to store and remember the last monitor used
    game_state->volume = 50; // TODO this need to be stored too

    // Determine the number of displayes
    game_state->display_id = SDL_GetDisplays(&game_state->n_displays);

    if(game_state->display_id == NULL){
        error("main.c : SDL could not detect displays! Error: %s", SDL_GetError());
        return 0;
    }

    info("main.c : There are %d displays connected!", game_state->n_displays);

    // Pick window size based on desktop
    // TODO we are not really using mode right now.
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(game_state->display_id[game_state->monitor]); 
    
    if(mode == NULL){
    	error("main.c : SDL cound not get Diaply mode: %s", SDL_GetError());
        return 0;
    }

    info("main.c : Create Display (%d x %d)[%fHz]", mode->w, mode->h, mode->refresh_rate);

    // Create a SDL window and renderer // TODO The window is forced to 1920x180 for right now
    if(!SDL_CreateWindowAndRenderer("game", 1920, 1080, SDL_WINDOW_RESIZABLE, &game_state->window, &game_state->renderer)){
    	error("main.c : SDL cond not create window! Error: %s", SDL_GetError());
        return 0;
    }

    SDL_GetWindowSize(game_state->window, &game_state->display_w, &game_state->display_h);

    game_state->surface = SDL_GetWindowSurface(game_state->window);
    
    if(game_state->surface == NULL){
        error("main.c : Could not get window surface! Error: %s", SDL_GetError());
    	software_window(game_state);
		exit(0);
	}

    info("main.c : Created game window and renderer! :D");

    // Load data
    game_state = loader(game_state);

    // Enter the menu state's loop
    info("main.c : Go to main menu");
    menu_state(game_state);

    // Save gamestate for next launch
    char data[256];
    memset(data, '\0', 256);
    sprintf(data, "display,resoultion,volume\n%d\n%d\n%d", game_state->monitor, -1, game_state->volume);

    FILE* out = jaccess("./game_state", "w+");
    
    if(out == NULL)
        error("main.c : Could not write file");
    else{
        info("main.c : Saving gamestate to disk");
        jwrite(out, data);
        fclose(out);
    }

	cleanup(game_state);
	game_state = NULL;
    
    info("main.c : Goodbye!");

    return 0;
}

void cleanup(Jgame* game_state){
    info("main.c : Cleaning up data");
	if(game_state->data_pack != NULL)
    	free_data(game_state);

    info("main.c : Quit SDL");
    // Cleanup
    SDL_free(game_state->display_id);
    SDL_DestroySurface(game_state->surface);
    SDL_DestroyWindow(game_state->window);
    SDL_DestroyRenderer(game_state->renderer);
    SDL_Quit();

    free(game_state);
    
}

void software_window(Jgame* game_state){
	warn("main.c : Trying to recover window now . . .");

	cleanup(game_state);
	game_state = NULL;

	debug("main.c : Query hint: %s", SDL_GetHint(SDL_HINT_RENDER_DRIVER));
	SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "software", SDL_HINT_OVERRIDE);
	debug("main.c : Setting hint to: %s", SDL_GetHint(SDL_HINT_RENDER_DRIVER));
	
	main(0, NULL);
}

Jgame* resize_window(Jgame* game_state){
    SDL_GetWindowSize(game_state->window, &game_state->display_w, &game_state->display_h);
    info("main.c : Update window size to %dx%d", game_state->display_w, game_state->display_h);

    SDL_DestroySurface(game_state->surface);
    game_state->surface = NULL;
    game_state->surface = SDL_GetWindowSurface(game_state->window);

    if(game_state->surface == NULL)
        error("main.c : Could not get window surface! Error: %s", SDL_GetError());

    return game_state;
}

