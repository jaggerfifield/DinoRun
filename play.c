#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "data_test.h"

#include "main.h"
#include "font.h"
#include "jdata.h"
#include "image.h"

static int state;
static int timer = 3000;
int score_var = 0;
int hiscore_var = 0;

bool up = false;
bool down = true;
int active[4] = {0, 0, 0 ,0};
int distance = 0;

static void update(SDL_Window*, struct Jdata*);
static void handle_keys(SDL_Event);
static void object_handler(struct Jdata*, SDL_Surface*);
static bool check_collision(SDL_Rect, SDL_Rect);
int time_left(int);


int play_state(SDL_Window* window, struct Jdata* data){
	state = PLAY;
	score_var = 0;
	
	int next_time = SDL_GetTicks() + 5;

	// Apply a seed for random
	time_t t;
	srand((unsigned) time(&t));

	color_key(find_node(data, ID_PLAY_PLAYER));
	color_key(find_node(data, ID_PLAY_OBJECT2));

	SDL_Event e;

	while(state == PLAY){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				state = EXIT;
			}else if(e.type == SDL_KEYDOWN){
				handle_keys(e);
			}
		}

		if(SDL_GetTicks() > next_time){
			update(window, data);
			next_time = next_time + 5;
		}
	}
	return state;

}

static void update(SDL_Window* window, struct Jdata* data){
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);


	if(timer >= -200){
		struct Jfont* temp = NULL;

		if(timer == 3000)
			temp = find_node(data, ID_PLAY_3);
		else if(timer == 2000)
			temp = find_node(data, ID_PLAY_2);
		else if(timer == 1000)
			temp = find_node(data, ID_PLAY_1);
		else if(timer == 0)
			temp = find_node(data, ID_PLAY_GO);

		if(temp != NULL){
			// Clear the screen first!
			struct Jimage* bg = find_node(data, ID_PLAY_BACKGROUND);
			SDL_BlitSurface(bg->img, NULL, win_surface, &bg->rect);
			
			// Blit the countdown
			SDL_BlitSurface(temp->img, NULL, win_surface, &temp->rect);
		}
		timer = timer - 500;
	}else{
		// Here is the dino run loop (after the countdown)
		struct Jfont* score = find_node(data, ID_PLAY_SCORE);
		struct Jfont* hiscore = find_node(data, ID_PLAY_HISCORE);
		struct Jimage* bg = data->data;
		struct Jimage* dino = find_node(data, ID_PLAY_PLAYER);

		int gravity = 4;

		// Update score count here.
		score_var += 1;
		char str[64];
		sprintf(str, "SCORE: %d", (int)(score_var/10));
		score->text = str;
		score->img = render_font(score);

		// Update HiScore here
		if(score_var > hiscore_var){
			char str[64];
			sprintf(str, "High Score: %d", (int)(score_var/10));
			hiscore->text = str;
			hiscore->img = render_font(hiscore);
			hiscore->rect.x = WINDOW_WIDTH - hiscore->img->w;
		}

		// Apply jump physics (up/down movement)

		// Move dino up
		if(!down && up && dino->rect.y > WINDOW_HEIGHT - dino->rect.h * 4)
			dino->rect.y = dino->rect.y - (gravity * 2);
		else{
			down = true;
			up = false;
		}

		// Move dino back down
		if(dino->rect.y < WINDOW_HEIGHT - dino->rect.h && down)
			dino->rect.y = dino->rect.y + (gravity);
		else
			down = false;

		// Blit the surfaces in order: bg, objects, score, player
		SDL_BlitSurface(bg->img, NULL, win_surface, &bg->rect);

		// Update object position and generate new objects
		object_handler(data, win_surface);
		

		SDL_BlitSurface(score->img, NULL, win_surface, &score->rect);
		SDL_BlitSurface(hiscore->img, NULL, win_surface, &hiscore->rect);
		SDL_BlitSurface(dino->img, NULL, win_surface, &dino->rect);
	}
	
	SDL_UpdateWindowSurface(window);
}

static void object_handler(struct Jdata* data, SDL_Surface* win_surface){
	int num = rand() % 128;

	if(distance != 0){
		distance = distance - 1;
	}

	int difficulity = 2;
	if(num < difficulity && ( (distance == 0) || (distance > 90) ) ){
		active[num] = 1;
		distance = 120;
	}

	// Move and blit objects.
	for(int i = 0; i < 4; i++){
		struct Jimage* obj = find_node(data, ID_PLAY_OBJECT + i);
		if(obj != NULL && active[i]){
			SDL_Rect pos;
			obj->rect.y = WINDOW_HEIGHT - obj->rect.h;
			obj->rect.x = obj->rect.x - 6;

			SDL_Rect player = ((struct Jimage*)find_node(data, ID_PLAY_PLAYER) )->rect;
			
			// Detect collision and end play state if true
			// TODO cleanup data and show a game over screen?
			if(check_collision(obj->rect, player))
				state = MENU;

			if(obj->rect.x < -obj->rect.w){
				obj->rect.x = WINDOW_WIDTH;
				active[i] = 0;
			}

			pos = obj->rect;
			SDL_BlitSurface(obj->img, NULL, win_surface, &pos);
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

static void handle_keys(SDL_Event e){
	int key = e.key.keysym.sym;
	if(key == SDLK_UP){
		up = true;
	}
}
