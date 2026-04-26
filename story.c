#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "main.h"
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

	struct Jdata** DTA = game_state->data_pack;

	// Enable background render on debug layer
	set_text_bg(DTA[ID_PLAY_DEBUG]);
	set_bgColour(DTA[ID_PLAY_DEBUG], 190, 190, 190);
	set_font_size(DTA[ID_PLAY_DEBUG], 10);

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
			}else if(e.type == SDL_EVENT_WINDOW_RESIZED){
                game_state = resize_window(game_state);
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
	
	timer_free(fpsTimer);
}

static void update(Jgame* game_state, struct Jdata** data){
	
	// Here is the dino run loop (after the countdown)
	struct Jdata* bg = data[ID_DATA_BACKGROUND];
	struct Jdata* dino = data[ID_PLAY_PLAYER];
	struct Jdata* debug = data[ID_PLAY_DEBUG];

	int gravity = 4;

	// Move dino up
	if(!down && up && get_pos_y(dino) > 0)
		set_pos_y(dino, get_pos_y(dino) - gravity, game_state);
	else{
		down = true;
		up = false;
	}

	// Move dino back down
	if(get_pos_y(dino) < game_state->display_h - dino->data.data->h && down)
		set_pos_y(dino, get_pos_y(dino) + (gravity), game_state);
	else
		down = false;

	// Move left and right
	if(right && !left && get_pos_x(dino) < game_state->display_w)
		set_pos_x(dino, get_pos_x(dino) + 1, game_state);

	if(left && !right && get_pos_x(dino) > 0)
		set_pos_x(dino, get_pos_x(dino) - 1, game_state);

	bg->pram.rect = get_rect(bg);
	dino->pram.rect = get_rect(dino);

	// Blit the surfaces in order: bg, objects, score, player
	SDL_BlitSurface(bg->data.data, NULL, game_state->surface, bg->pram.rect);

	SDL_BlitSurface(dino->data.data,    NULL, game_state->surface, dino->pram.rect);
		
	// Blit debug overlay if enabled
	if(debug_overlay){
		float avgFPS = frameCount / (timer_getTicks(fpsTimer) / 1000.f);

		if(avgFPS > 2000000)
			avgFPS = 0;
		
        set_string(debug, "FPS: %.3f", avgFPS);
		render(debug);
			
		debug->pram.rect = get_rect(debug);

		SDL_BlitSurface(debug->data.data, NULL, game_state->surface, debug->pram.rect);
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
