#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "main.h"
#include "jdata.h"
#include "jtime.h"
#include "jio.h"

// TODO: movement struct, remove the global varialbes.
static bool up = false;
static bool down = true;
static bool left = false;
static bool right = false;

static bool debug_overlay = false;
static unsigned int frameCount = 0;
static struct Jtimer* fpsTimer;

static void update(Jgame*, struct Jdata**);
static void handle_keys(SDL_KeyboardEvent);
static void handle_keyup(SDL_KeyboardEvent);

void story_state(Jgame* game_state){

	fpsTimer = timer_init();

	int dSize = 3;

    SDL_Window* window = game_state->window; // TODO this is temp for data loading, this will move to data.c

	struct Jdata* DTA[dSize];
	DTA[0] = init(ID_PLAY_BACKGROUND, JIMAGE, 0, 0, "Play background", "Assets/bg_fill.bmp", NULL);
	DTA[1] = init(ID_PLAY_PLAYER, JIMAGE, 25, 100, "Player", "Assets/image.bmp", NULL);
	// Debug layers
	DTA[2] = init(912, JFONT, 0, 0, "Debug overlay", "Assets/font.ttf", "");

	// Enable background render on debug layer
	set_text_bg(DTA[2]);
	set_bgColour(DTA[2], 190, 190, 190);
	set_text_size(DTA[2], 10);

	int next_time = SDL_GetTicks() + 5;

	// Apply a seed for random
	time_t t;
	srand((unsigned) time(&t));

	SDL_Event e;

	// Start fps timer
	timer_start(fpsTimer);
	frameCount = 0;

	bool quit = false;

	// Play loop
	while(!quit){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				quit = true;
			}else if(e.type == SDL_EVENT_KEY_DOWN){
				handle_keys(e.key);
			}else if(e.type == SDL_EVENT_KEY_UP){
				handle_keyup(e.key);
			}
		}

		if(SDL_GetTicks() > next_time){
			update(game_state, DTA);
			next_time = next_time + 5;
		}
	}
	
	// Free datas
	for(int i = 0; i < dSize; i++){
		jdata_free(DTA[i]);
	}

	timer_free(fpsTimer);
}

static void update(Jgame* game_state, struct Jdata** data){
	
	// Here is the dino run loop (after the countdown)
	struct Jdata* bg = data[0];
	struct Jdata* dino = data[1];
	struct Jdata* debug = data[2];

	int gravity = 4;

	// Move dino up
	if(!down && up && dino->y > 0)
		dino->y = dino->y - gravity;
	else{
		down = true;
		up = false;
	}

	// Move dino back down
	if(dino->y < game_state->display_h - dino->data->h && down)
		dino->y = dino->y + (gravity);
	else
		down = false;

	// Move left and right
	if(right && !left && dino->x < game_state->display_w)
		dino->x = dino->x + 1;

	if(left && !right && dino->x > 0)
		dino->x = dino->x - 1;

	SDL_Rect bg_rect = get_rect(bg, game_state);
	SDL_Rect dino_rect = get_rect(dino, game_state);

	// Blit the surfaces in order: bg, objects, score, player
	SDL_BlitSurface(bg->data, NULL, game_state->surface, &bg_rect);

	SDL_BlitSurface(dino->data,    NULL, game_state->surface, &dino_rect);
		
	// Blit debug overlay if enabled
	if(debug_overlay){
		float avgFPS = frameCount / (timer_getTicks(fpsTimer) / 1000.f);

		if(avgFPS > 2000000)
			avgFPS = 0;

        char str[32];
        memset(str, '\0', 32);

		// Update string
		sprintf(str, "FPS: %f", avgFPS);
		
		debug->string = str;
		render(debug);
			
		SDL_Rect debug_rect = get_rect(debug, game_state);

		SDL_BlitSurface(debug->data, NULL, game_state->surface, &debug_rect);
	}

	frameCount = frameCount + 1;
	
	SDL_UpdateWindowSurface(game_state->window);
}

static void handle_keys(SDL_KeyboardEvent e){
	int key = e.key;
	
	if(key == SDLK_UP){
		up = true;
	}else if(key == SDLK_LEFT){
		left = true;
	}else if(key == SDLK_RIGHT){
		right = true;
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		info("Toggle debug overlay to: %d", debug_overlay);
	}
}

static void handle_keyup(SDL_KeyboardEvent e){
	int key = e.key;

	if(key == SDLK_LEFT){
		left = false;
	}else if(key == SDLK_RIGHT){
		right = false;
	}
}
