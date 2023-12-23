#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include<stdbool.h>
#include <time.h>

#include "main.h"
#include "jdata.h"
#include "jtime.h"
#include "jio.h"
#include "gameover.h"

static int h,w;
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
int distance = 0;

static void update(SDL_Window*, struct Jdata**);
static void handle_keys(SDL_Event);
static void object_handler(struct Jdata**, SDL_Window*);
static bool check_collision(SDL_Rect, SDL_Rect);
void read_score(void);
void write_score(void);

int time_left(int);

void play_state(SDL_Window* window){
	SDL_GetWindowSize(window, &h, &w);
	if(h<0||w<0){
		error("Bad window Size!");
		exit(-1);
	}

	score_var = 0;
	
	read_score();

	fpsTimer = timer_init();

	int dSize = 13;

	struct Jdata* DTA[dSize];
	DTA[0] = init(ID_PLAY_BACKGROUND, JIMAGE, 0, 0, "Play background", "Assets/bg_fill.bmp", NULL, window);
	DTA[1] = init(ID_PLAY_PLAYER, JIMAGE, 25, 100, "Player", "Assets/image.bmp", NULL, window);
	DTA[2] = init(ID_PLAY_3, JFONT, CENTER, CENTER, "Countdown 3", "Assets/font.ttf", "3", window);
	DTA[3] = init(ID_PLAY_2, JFONT, CENTER, CENTER, "Countdown 2", "Assets/font.ttf", "2", window);
	DTA[4] = init(ID_PLAY_1, JFONT, CENTER, CENTER, "Countdown 1", "Assets/font.ttf", "1", window);
	DTA[5] = init(ID_PLAY_GO, JFONT, CENTER, CENTER, "Countdown GO", "Assets/font.ttf", "GO!", window);
	DTA[6] = init(ID_PLAY_SCORE, JFONT, 0, 0, "Score text", "Assets/font.ttf", "SCORE: 0", window);
	DTA[7] = init(ID_PLAY_HISCORE, JFONT, 0, 0, "Hiscore text", "Assets/font.ttf", "HISCORE: 0", window);
	DTA[8] = init(ID_PLAY_OBJECT, JIMAGE, w, 0, "Object 1", "Assets/dino.bmp", NULL, window);
	DTA[9] = init(ID_PLAY_OBJECT2, JIMAGE, w, 0, "Object 2", "Assets/ball.bmp", NULL, window);
	DTA[10] = init(ID_PLAY_OBJECT3, JIMAGE, w, 0, "Object 3", "Assets/OBJ3.bmp", NULL, window);
	DTA[11] = init(ID_PLAY_OBJECT4, JIMAGE, w, 0, "Object 4", "Assets/dino.bmp", NULL,window);
	// Debug layers
	DTA[12] = init(912, JFONT, 0, 0, "Debug overlay", "Assets/font.ttf", "", window);

	// Enable background render on debug layer
	set_text_bg(DTA[12]);
	set_bgColour(DTA[12], 190, 190, 190);
	set_text_size(DTA[12], 10);

	int next_time = SDL_GetTicks() + 5;

	// Apply a seed for random
	time_t t;
	srand((unsigned) time(&t));

	SDL_Event e;

	// Start fps timer
	timer_start(fpsTimer);
	frameCount = 0;

	bool quit = false;
	over = false;

	// Play loop
	while(!quit && !over){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				quit = true;
			}else if(e.type == SDL_KEYDOWN){
				handle_keys(e);
			}
		}

		if(SDL_GetTicks() > next_time){
			update(window, DTA);
			next_time = next_time + 5;
		}
	}
	
	// Free datas
	for(int i = 0; i < dSize; i++){
		jdata_free(DTA[i]);
	}

	timer_free(fpsTimer);
	
	if(!quit)
		gameover_state(window);
}

static void update(SDL_Window* window, struct Jdata** data){

	SDL_Surface* win_surface = SDL_GetWindowSurface(window);
	
	// Count down timer TODO: find a better way to time things here
	if(timer >= -200){
		struct Jdata* temp = NULL;

		if(timer == 3000)
			temp = data[ID_PLAY_3];
		else if(timer == 2000)
			temp = data[ID_PLAY_2];
		else if(timer == 1000)
			temp = data[ID_PLAY_1];
		else if(timer == 0)
			temp = data[ID_PLAY_GO];

		if(temp != NULL){
			// Clear the screen first!
			struct Jdata* bg = data[ID_PLAY_BACKGROUND];
			SDL_Rect bg_rect = get_rect(bg);
			SDL_BlitSurface(bg->data, NULL, win_surface, &bg_rect);
				
			// Blit the countdown
			SDL_Rect temp_rect = get_rect(temp);
			SDL_BlitSurface(temp->data, NULL, win_surface, &temp_rect);
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

		// Update score count here.
		score_var += 1;
		char str[64];
		sprintf(str, "SCORE: %d", (int)(score_var/10));
		score->string = str;
		render(score);

		// Update HiScore here
		if(score_var > hiscore_var)
			hiscore_var = score_var;
		sprintf(str, "High Score: %d", (int)(hiscore_var/10));
		hiscore->string = str;
		render(hiscore);
		hiscore->x = w - hiscore->data->w;

		// Apply jump physics (up/down movement)

		// Move dino up
		if(!down && up && dino->y > 0)
			dino->y = dino->y - gravity;
		else{
			down = true;
			up = false;
		}

		// Move dino back down
		if(dino->y < h - dino->data->h && down)
			dino->y = dino->y + (gravity);
		else
			down = false;

		SDL_Rect bg_rect = get_rect(bg);
		SDL_Rect score_rect = get_rect(score);
		SDL_Rect hiscore_rect = get_rect(hiscore);
		SDL_Rect dino_rect = get_rect(dino);

		// Blit the surfaces in order: bg, objects, score, player
		SDL_BlitSurface(bg->data, NULL, win_surface, &bg_rect);

		// Update object position and generate new objects
		object_handler(data, window);		

		SDL_BlitSurface(score->data,   NULL, win_surface, &score_rect);
		SDL_BlitSurface(hiscore->data, NULL, win_surface, &hiscore_rect);
		SDL_BlitSurface(dino->data,    NULL, win_surface, &dino_rect);
		
		// Blit debug overlay if enabled
		if(debug_overlay){
			float avgFPS = frameCount / (timer_getTicks(fpsTimer) / 1000.f);

			if(avgFPS > 2000000)
				avgFPS = 0;

			// Update string
			sprintf(str, "FPS: %f", avgFPS);
			
			debug->string = str;
			render(debug);
			
			SDL_Rect debug_rect = get_rect(debug);

			SDL_BlitSurface(debug->data, NULL, win_surface, &debug_rect);
		}
	}

	frameCount = frameCount + 1;
	
	SDL_UpdateWindowSurface(window);
}

static void object_handler(struct Jdata** data, SDL_Window* window){
	SDL_Surface* win_surface = SDL_GetWindowSurface(window);

	int num = rand() % 128;

	if(distance != 0){
		distance = distance - 1;
	}

	// The number of objects allowed on the screen is determined by difficulty (2 means 2 objects)
	int difficulity = 2;

	// distance helps to prevent impossible jumps. (It counts down from 120 to 0 after a object is activated)
	if(num < difficulity && ( (distance == 0) || (distance > 95) ) ){
		active[num] = 1;
		distance = 120;
	}

	// Move and blit objects.
	for(int i = 0; i < 4; i++){
		struct Jdata* obj = data[ID_PLAY_OBJECT + i];
		if(obj != NULL && active[i]){
			obj->y = h - obj->data->h;
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
				if(score_var >= hiscore_var)
					write_score();
			}

			if(obj->x < -obj->data->w){
				obj->x = w;
				active[i] = 0;
			}

			SDL_BlitSurface(obj->data, NULL, win_surface, &object_rect);
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
	
	char msg[64];
	sprintf(msg, "Read score to be: %d", hiscore_var);
	debug(msg);
}

void write_score(){
	FILE* f = jaccess("score", "w");
	char content[64];
	
	sprintf(content, "%d\n", score_var);
	jwrite(f, content);
	fclose(f);

	sprintf(content, "Recording new hiscore: %d", score_var);
	debug(content);
}

static void handle_keys(SDL_Event e){
	int key = e.key.keysym.sym;
	
	if(key == SDLK_UP || key == SDLK_SPACE){
		up = true;
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		char tmp[64];
		sprintf(tmp, "Toggle debug overlay to: %d", debug_overlay);
		info(tmp);
	}
}
