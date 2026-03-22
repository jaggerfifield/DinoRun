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
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(game_state->display_id[game_state->monitor]); 
    
    if(mode == NULL){
    	error("main.c : SDL cound not get Diaply mode: %s", SDL_GetError());
        return 0;
    }

    info("main.c : Create Display (%d x %d)[%fHz]", mode->w, mode->h, mode->refresh_rate);

    // Create a SDL window and renderer
    if(!SDL_CreateWindowAndRenderer("game", mode->w, mode->h, SDL_WINDOW_RESIZABLE, &game_state->window, &game_state->renderer)){
    	error("main.c : SDL cond not create window! Error: %s", SDL_GetError());
        return 0;
    }
    
    SDL_GetWindowSize(game_state->window, &game_state->display_w, &game_state->display_h);

    game_state->surface = SDL_GetWindowSurface(game_state->window);
    
    if(game_state->surface == NULL){
        error("main.c : Could not get window surface! Error: %s", SDL_GetError());
    }

    info("main.c : Created game window and renderer! :D");

    // Load data
    loader(game_state->window);

    // Enter the menu state's loop
    info("main.c : Go to main menu");
    menu_state(game_state);

    // Cleanup
    SDL_DestroyWindow(game_state->window);
    SDL_DestroyRenderer(game_state->renderer);
    SDL_DestroySurface(game_state->surface);
    SDL_Quit();

    free(game_state);

    return 0;
}

