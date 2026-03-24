#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <time.h>

#include "main.h"
#include "jdata.h"
#include "jtime.h"
#include "jio.h"
#include "gameover.h"
#include "update.h"

static int timer = 3000;
int score_var = 0;
int hiscore_var = 0;

bool debug_overlay = false;
unsigned int frameCount = 0;
struct Jtimer* fpsTimer;

bool over = false;
bool up = false;
bool down = true;
int active[4] = {0, 0, 0 ,0};
int distance = 100;

static void _update(Jgame*, struct Jdata**);
static void handle_keys(SDL_KeyboardEvent);
static void object_handler(struct Jdata**, Jgame*);
static bool check_collision(SDL_Rect, SDL_Rect);
void read_score(void);
void write_score(void);

int time_left(int);

void play_state(Jgame* game_state){
	score_var = 0;
	
	read_score();

	fpsTimer = timer_init();

    struct Jdata** DTA = game_state->data_pack[1];

	// Enable background render on debug layer
	set_text_bg(DTA[ID_PLAY_DEBUG]);
	set_bgColour(DTA[ID_PLAY_DEBUG], 190, 190, 190);
	set_text_size(DTA[ID_PLAY_DEBUG], 10);

	int next_time = SDL_GetTicks() + 5;

	// Apply a seed for random
	srand(time(NULL));

	SDL_Event e;

	// Start fps timer
	timer_start(fpsTimer);
	frameCount = 0;

	bool quit = false;
	over = false;

	// Play loop
	while(!quit && !over){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				quit = true;
			}else if(e.type == SDL_EVENT_KEY_DOWN){
				handle_keys(e.key);
			}
		}

		if(SDL_GetTicks() > next_time){
			_update(game_state, DTA);
			next_time = next_time + 5;
		}
	}
	
	timer_free(fpsTimer);
	
	if(!quit)
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
		struct Jdata* debug = data[12];

		int gravity = 4;
		int accel = 1;

		// Update score count here.
		score_var += 1;
		sprintf(score->string, "SCORE: %d", (int)(score_var/10));
		render(score);

		// Update HiScore here
		if(score_var > hiscore_var)
			hiscore_var = score_var;
		sprintf(hiscore->string, "High Score: %d", (int)(hiscore_var/10));
		render(hiscore);
		hiscore->x = game_state->display_w - hiscore->data->w;

		// Apply jump physics (up/down movement)

		// Move dino up
		if( ( !down && up ) && ( dino->y > (game_state->display_h - (game_state->display_h/2)) ) ) 
			dino->y = dino->y - gravity*2;
		else{
			down = true;
			up = false;
		}

		// Move dino back down
		if(dino->y < game_state->display_h - dino->data->h && down){
			dino->y = dino->y + (gravity+accel);
			accel += 1;
		}else{
			accel = 1;
			down = false;
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
	int num = rand() % 4;

	if(distance != 0){
		distance = distance - 1;
	}

	// The number of objects allowed on the screen is determined by difficulty (2 means 2 objects)
	int difficulity = 4;

	// distance helps to prevent impossible jumps. (It counts down from 120 to 0 after a object is activated)
	if(num < difficulity && ( (distance == 0) ) ) {//|| (distance > 95) ) ){
		active[num] = 1;
		distance = 120;
	}

	// Move and blit objects.
	for(int i = 0; i < 4; i++){
		struct Jdata* obj = data[ID_PLAY_OBJECT + i];
		if(obj != NULL && active[i]){
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
				over = true;

				for(int k = 0; k < 4; k++){
					active[k] = 0;
				}

				if(score_var >= hiscore_var)
					write_score();
			}

			if(obj->x < -obj->data->w){
				obj->x = game_state->display_w;
				active[i] = 0;
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

void read_score(){
	char value[9] = "00000000\0";
	FILE* f = jaccess("score", "r");
	jread(f, value, 8);
	fclose(f);
	sscanf(value, "%d", &hiscore_var);
	
	debug("Read score to be: %d", hiscore_var);
}

void write_score(){
	FILE* f = jaccess("score", "w");
	char content[64];
	
	sprintf(content, "%d\n", score_var);
	jwrite(f, content);
	fclose(f);

	debug("Recording new hiscore: %d", score_var);
}

static void handle_keys(SDL_KeyboardEvent e){
	SDL_Keycode key = e.key;
	
	if(key == SDLK_UP || key == SDLK_SPACE){
		up = true;
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		info("Toggle debug overlay to: %d", debug_overlay);
	}
}
