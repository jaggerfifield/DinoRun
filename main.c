#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>

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
    	error("SDL cound not init! Error: %s", SDL_GetError());
        return 0;
    }
    if(!TTF_Init()){
    	error("TTF could not init! Error: %s", SDL_GetError());
        return 0;
    }

    info("Start program!");
 
    // Determine the number of displayes
    int n_displays = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&n_displays);

    if(displays == NULL){
        error("SDL could not detect displays! Error: %s", SDL_GetError());
        return 0;
    }

    info("There are %d displays connected!", n_displays);

    // Pick window size based on desktop
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[0]); // TODO we pick the first display we find, need to add a select display in settings.c and make this pick the last used display.
    if(mode == NULL){
    	error("SDL cound not get Diaply mode: %s", SDL_GetError());
        return 0;
    }

    info("Create Display (%d x %d)[%fHz]", mode->w, mode->h, mode->refresh_rate);

    // Create a SDL window and renderer
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    if(!SDL_CreateWindowAndRenderer("game", mode->w, mode->h, SDL_WINDOW_RESIZABLE, &window, &render)){
    	error("SDL cond not create window! Error: %s", SDL_GetError());
        return 0;
    }

    // Init the window for updates
    init_update(window);

    // Load data
    loader(window);

    // Enter the menu state's loop
    info("Go to main menu");
    menu_state(window, render);

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    window = NULL;
    render = NULL;
    SDL_Quit();

    return 0;
}

