#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>

#include "jio.h"
#include "jdata.h"
#include "data.h"
#include "menu.h"
#include "update.h"

bool init_systems();
bool get_display(void);

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
 
    // Determine the number of displayes
    //if(!get_display())
    //   return 0;

    int n_displays = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&n_displays);

    if(displays == NULL){
        error("SDL could not detect displays! Error: %s", SDL_GetError());
        return 0;
    }

    info("There are %d displays connected!", n_displays);

    // Pick window size based on desktop
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[0]);
    if(mode == NULL){
    	error("SDL cound not get Diaply mode: %s", SDL_GetError());
        return 0;
    }

    info("Create Display (%d x %d)[%fHz]", mode->w, mode->h, mode->refresh_rate);

    if(1920/1080!=mode->w/mode->h){
    	error("Incompatible resolution (%d x %d)!", mode->w, mode->h);
        return 0;
    }

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

    info("Go to main menu");

    // Enter the menu state's loop
    menu_state(window, render);

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    window = NULL;
    render = NULL;
    SDL_Quit();
    return 0;
}

bool init_systems(){
    bool success = 1;

    if(!SDL_Init(SDL_INIT_VIDEO)){
    	success = 0;
    	error("SDL cound not init! Error: %s", SDL_GetError());
    }
	
    if(!TTF_Init()){
    	success = 0;
    	error("TTF could not init! Error: %s", SDL_GetError());
    }

    return success;
}

bool get_display( void ){
    bool success = 1;
    //SDL_DisplayMode* mode;
    //SDL_DisplayMode curr;

    //if(SDL_GetDesktopDisplayMode(0, &curr) != 0){
    //	error("SDL cound not get display mode: %s", SDL_GetError());
    //}

    //Uint32 g = curr.format;
    //SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i (%iHz) [%d]", g, SDL_BITSPERPIXEL(g),SDL_GetPixelFormatName(g), curr.w, curr.h, curr.refresh_rate, curr.w/curr.h==1920/1080);

    /*
    int display_count = SDL_GetNumVideoDisplays();
    info("Found %d displays!", display_count);

    for(int i = 0; i < display_count; i++){
        int display_modes = SDL_GetNumDisplayModes(i);
	
        info("=====DISPLAY %d=====", i);
	
        for(int j = 0; j < display_modes; j++){
	        if(SDL_GetDisplayMode(i, j, &mode) != 0){
		        error("SDL display mode error! %s", SDL_GetError());
		        success = 0;
	        }
	    
            Uint32 f = mode.format;

            SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i (%iHz) [%d]", i, SDL_BITSPERPIXEL(f),SDL_GetPixelFormatName(f), mode.w, mode.h, mode.refresh_rate, mode.w/mode.h==1920/1080);
	    }
    }
    */
    return success;
}

