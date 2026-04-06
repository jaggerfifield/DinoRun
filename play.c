#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <time.h>

#include "main.h"
#include "jtime.h"
#include "jio.h"
#include "gameover.h"
#include "update.h"

static int timer = 3000;

bool debug_overlay = false;
unsigned int frameCount = 0;
struct Jtimer* fpsTimer;

int distance = 100;

static void _update(Jgame*, struct Jdata**);
static void handle_keys(SDL_KeyboardEvent, Jgame*);
static void object_handler(struct Jdata**, Jgame*);
static bool check_collision(SDL_Rect, SDL_Rect);
void read_score(Jgame*);
void write_score(Jgame*);

int time_left(int);

enum {IDLE, UP, DOWN};

void play_state(Jgame* game_state){
	game_state->score = 0;
	game_state->game_over = false;
	game_state->objects = 4;
    game_state->motion = IDLE;

	// TODO this is a temp key map, this nees to be set elsewhere!
	game_state->jump1 = SDLK_UP;

	read_score(game_state);

	fpsTimer = timer_init();

    struct Jdata** DTA = game_state->data_pack[1];

	// Enable background render on debug layer
	set_text_bg(DTA[ID_PLAY_DEBUG]);
	set_bgColour(DTA[ID_PLAY_DEBUG], 190, 190, 190);
	set_text_size(DTA[ID_PLAY_DEBUG], 10);

	// Set object positions
    DTA[ID_PLAY_OBJECT]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT2]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT3]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT4]->x = game_state->display_w;
	DTA[ID_PLAY_PLAYER]->y = game_state->display_h-DTA[ID_PLAY_PLAYER]->data->h;

	game_state->jump_height = game_state->display_h - DTA[ID_PLAY_PLAYER]->data->h - 250;
	
    int next_time = SDL_GetTicks() + 5;

	// Apply a seed for random
	srand(time(NULL));

	SDL_Event e;

	// Start fps timer
	timer_start(fpsTimer);
	frameCount = 0;

	// Play loop
	while(!game_state->quit && !game_state->game_over){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				game_state->quit = true;
			}else if(e.type == SDL_EVENT_WINDOW_RESIZED){
                game_state = resize_window(game_state);
            }else if(e.type == SDL_EVENT_KEY_DOWN){
				handle_keys(e.key, game_state);
			}
		}

        //if(SDL_GetTicks() > next_time){
            _update(game_state, DTA);
        //    next_time = next_time + 10;
        //}

    }
	
	timer_free(fpsTimer);
	
	if(!game_state->quit)
		gameover_state(game_state);
}

static void _update(Jgame* game_state, struct Jdata** data){

	// Count down timer TODO: find a better way to time things here
	if(timer >= -200){
		struct Jdata* timer_node = data[ID_PLAY_TIMER];

		if(timer == 2000)
			sprintf(timer_node->string, "2");
		else if(timer == 1000)
			sprintf(timer_node->string, "1");
		else if(timer == 0)
            sprintf(timer_node->string, "Go!");

        render(timer_node); // TODO this is called too many times

		if(timer_node != NULL){
			// Clear the screen first!
			struct Jdata* bg = data[ID_PLAY_BACKGROUND];
			SDL_Rect bg_rect = get_rect(bg, game_state);
			SDL_BlitSurface(bg->data, NULL, game_state->surface, &bg_rect);
				
			// Blit the countdown
			SDL_Rect temp_rect = get_rect(timer_node, game_state);
			SDL_BlitSurface(timer_node->data, NULL, game_state->surface, &temp_rect);
		}
		timer = timer - 5;

	// This is the main update, where we hande the gameplay
	}else{
		// Here is the dino run loop (after the countdown)
		struct Jdata* score = data[ID_PLAY_SCORE];
		struct Jdata* hiscore = data[ID_PLAY_HISCORE];
		struct Jdata* bg = data[ID_PLAY_BACKGROUND];
		struct Jdata* dino = data[ID_PLAY_PLAYER];
		struct Jdata* debug = data[ID_PLAY_DEBUG];

		int gravity = 4;

		// Update score count here.
		game_state->score += 1;
		sprintf(score->string, "SCORE: %d", (int)(game_state->score/10));
		render(score);

		// Update HiScore here
		if(game_state->score > game_state->hiscore)
			game_state->hiscore = game_state->score;
		sprintf(hiscore->string, "High Score: %d", (int)(game_state->hiscore/10));
		render(hiscore);
		hiscore->x = game_state->display_w - hiscore->data->w;

		// Apply jump physics (up/down movement)

		switch(game_state->motion){
			case IDLE:
				break;
			case UP:
				if(dino->y > game_state->jump_height)
					dino->y = dino->y - 4;
				else
				  	game_state->motion = DOWN;
				break;
			case DOWN:
			  	if(dino->y < game_state->display_h - dino->data->h)
					dino->y = dino->y + gravity;
			  	else if(dino->y >= game_state->display_h - dino->data->h)
				  	game_state->motion = IDLE;
			  	break;
		}
		
		SDL_Rect bg_rect = get_rect(bg, game_state);
		SDL_Rect score_rect = get_rect(score, game_state);
		SDL_Rect hiscore_rect = get_rect(hiscore, game_state);
		SDL_Rect dino_rect = get_rect(dino, game_state);

		// Blit the surfaces in order: bg, objects, score, player
		SDL_BlitSurface(bg->data, NULL, game_state->surface, &bg_rect);

		// Update object position and generate new objects
		object_handler(data, game_state);		

		SDL_BlitSurface(score->data,   NULL, game_state->surface, &score_rect);
		SDL_BlitSurface(hiscore->data, NULL, game_state->surface, &hiscore_rect);
		SDL_BlitSurface(dino->data,    NULL, game_state->surface, &dino_rect);
		
		// Blit debug overlay if enabled
		if(debug_overlay){
			float avgFPS = frameCount / (timer_getTicks(fpsTimer) / 1000.f);

			if(avgFPS > 2000000)
				avgFPS = 0;

			// Update string
			sprintf(debug->string, "FPS: %f", avgFPS);
			render(debug);
			
			SDL_Rect debug_rect = get_rect(debug, game_state);

			SDL_BlitSurface(debug->data, NULL, game_state->surface, &debug_rect);
		}
	}

	frameCount = frameCount + 1;
	
	SDL_UpdateWindowSurface(game_state->window);
}

static void object_handler(struct Jdata** data, Jgame* game_state){
	int num = rand() % game_state->objects;

	if(distance != 0){
		distance = distance - 1;
	}

	// distance helps to prevent impossible jumps. (It counts down from 120 to 0 after a object is activated)
	if(distance == 0) {//|| (distance > 95) ) ){
		game_state->obstacle[num] = 1;
		distance = 120;
	}

	// Move and blit objects.
	for(int i = 0; i < game_state->objects; i++){
		struct Jdata* obj = data[ID_PLAY_OBJECT + i];
		if(obj != NULL && game_state->obstacle[i]){
			obj->y = game_state->display_h - obj->data->h;
			obj->x = obj->x - 6;


			// Generate rects
			SDL_Rect player_rect;
			struct Jdata* player = data[ID_PLAY_PLAYER];
			
			player_rect.x = player->x;
			player_rect.y = player->y;
			player_rect.h = player->data->h;
			player_rect.w = player->data->w;

			SDL_Rect object_rect;

			object_rect.x = obj->x;
			object_rect.y = obj->y;
			object_rect.h = obj->data->h;
			object_rect.w = obj->data->w;

			// Detect collision and end play state if true
			if(check_collision(object_rect, player_rect)){
				game_state->game_over = true;

				// Reset active objects
				for(int k = 0; k < 4; k++){
					game_state->obstacle[k] = 0;
				}

				if(game_state->score > game_state->hiscore)
					write_score(game_state);
			}

			// Once object is off the screen we turn it off
			if(obj->x < -obj->data->w){
				obj->x = game_state->display_w;
				game_state->obstacle[i] = 0;
			}

			SDL_BlitSurface(obj->data, NULL, game_state->surface, &object_rect);
		}
	}
}

static bool check_collision(SDL_Rect a, SDL_Rect b){
	if(a.y + a.h <= b.y)
		return false;
	if(a.y >= b.y + b.h)
		return false;
	if(a.x + a.w <= b.x)
		return false;
	if(a.x >= b.x + b.w)
		return false;
	return true;
}

void read_score(Jgame* game_state){
	char value[9] = "00000000\0";
	FILE* f = jaccess("score", "r");
	jread(f, value, 8);
	fclose(f);
	sscanf(value, "%d", &game_state->hiscore);
	
	debug("Read score to be: %d", game_state->hiscore);
}

void write_score(Jgame* game_state){
	FILE* f = jaccess("score", "w");
	char content[64];
	
	sprintf(content, "%d\n", game_state->score);
	jwrite(f, content);
	fclose(f);

	debug("Recording new hiscore: %d", game_state->score);
}

void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){
	SDL_Keycode key = e.key;
	
	if(key == game_state->jump1 || key == game_state->jump2 || key == game_state->jump3){
		game_state->motion = UP;
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		info("Toggle debug overlay to: %d", debug_overlay);
	}
}
