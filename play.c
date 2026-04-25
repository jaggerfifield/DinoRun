#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <time.h>

#include "main.h"
#include "jio.h"
#include "gameover.h"

bool debug_overlay = false;
int distance = 100;

static void _update(Jgame*, struct Jdata**);
static void handle_keys(SDL_KeyboardEvent, Jgame*);
static void object_handler(struct Jdata**, Jgame*);
static bool check_collision(SDL_Rect*, SDL_Rect*);
void read_score(Jgame*);
void write_score(Jgame*);

enum {IDLE, UP, DOWN};

void play_state(Jgame* game_state){
	game_state->score = 1;
	game_state->game_over = false;
	game_state->objects = 4;
    game_state->motion = IDLE;
    game_state->point_mult = 1;
    game_state->coins = 0;
    game_state->coin_get = true; 

    for(int i = 0; i < 10; i++){
        game_state->obstacle[i] = 0;
        game_state->platform[i] = 0;
        game_state->treasure[i] = 0;
    }

	// TODO this is a temp key map, this nees to be set elsewhere!
	game_state->jump1 = SDLK_UP;
    game_state->game_speed = 20; // Speed multiplier TODO

	read_score(game_state);

    struct Jdata** DTA = game_state->data_pack;

	// Enable background render on debug layer
	set_text_bg(DTA[ID_PLAY_DEBUG]);
	set_bgColour(DTA[ID_PLAY_DEBUG], 190, 190, 190);
	set_font_size(DTA[ID_PLAY_DEBUG], 10);

	// Set object positions
    DTA[ID_PLAY_COINS]->y = DTA[ID_PLAY_SCORE]->data->h+10;
    
    DTA[ID_PLAY_OBJECT]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT2]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT3]->x = game_state->display_w;
    DTA[ID_PLAY_OBJECT4]->x = game_state->display_w;
	DTA[ID_PLAY_PLAYER]->y = game_state->display_h-DTA[ID_PLAY_PLAYER]->data->h;
    
    DTA[ID_PLAY_COIN]->y = game_state->display_h-DTA[ID_PLAY_PLAYER]->data->h-DTA[ID_PLAY_COIN]->data->h-50;
    DTA[ID_PLAY_COIN]->x = game_state->display_w;

	game_state->jump_height = game_state->display_h - DTA[ID_PLAY_PLAYER]->data->h - 280;
	
    set_string(DTA[ID_PLAY_HISCORE], "High Score: %d", game_state->hiscore);
    render(DTA[ID_PLAY_HISCORE]);
	DTA[ID_PLAY_HISCORE]->x = game_state->display_w - DTA[ID_PLAY_HISCORE]->data->w;

	// Apply a seed for random
	srand(time(NULL));

	SDL_Event e;

    game_state->time_tick = SDL_GetTicksNS();
    game_state->animation_tick = SDL_GetTicksNS();

	// Play loop
	while(!game_state->quit && !game_state->game_over){
		game_state->start_tick = SDL_GetTicksNS();

        while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				game_state->quit = true;
			}else if(e.type == SDL_EVENT_WINDOW_RESIZED){
                game_state = resize_window(game_state);
            }else if(e.type == SDL_EVENT_KEY_DOWN){
				handle_keys(e.key, game_state);
			}
		}
        
        _update(game_state, DTA);
        
        // Every time we hit a mutiple of 1000 we increase the game speed
        if(game_state->score % 1000 == 0){
            game_state->game_speed += 2;
        }
    }
	
	if(!game_state->quit)
		gameover_state(game_state);
}

static void _update(Jgame* game_state, struct Jdata** data){

	if ( ( SDL_GetTicksNS() - game_state->time_tick ) < 4000000000){
        struct Jdata* timer_node = data[ID_PLAY_TIMER];
        bool do_update = true;

        if( ( SDL_GetTicksNS() - game_state->time_tick ) < 1000000000 )
			set_string(timer_node, "3");
        else if( ( SDL_GetTicksNS() - game_state->time_tick ) < 2000000000 )
			set_string(timer_node, "2");
		else if( ( SDL_GetTicksNS() - game_state->time_tick ) < 3000000000 )
			set_string(timer_node, "1");
		else if( ( SDL_GetTicksNS() - game_state->time_tick ) < 4000000000 )
            set_string(timer_node, "Go!");
        else
            do_update = false;

        if(do_update){
            render(timer_node);

		    // Clear the screen first!
		    struct Jdata* bg = data[ID_DATA_BACKGROUND];
		    
            if(bg->rect == NULL)
                bg->rect = get_rect(bg, game_state);

		    SDL_BlitSurface(bg->data, NULL, game_state->surface, bg->rect);
				
		    // Blit the countdown
            if(timer_node->rect == NULL)
		        timer_node->rect = get_rect(timer_node, game_state);

		    SDL_BlitSurface(timer_node->data, NULL, game_state->surface, timer_node->rect);

        }
	// This is the main update, where we hande the gameplay
	}else{
		// Here is the dino run loop (after the countdown)
		struct Jdata* score = data[ID_PLAY_SCORE];
		struct Jdata* coin_t = data[ID_PLAY_COINS];
        struct Jdata* hiscore = data[ID_PLAY_HISCORE];
		struct Jdata* bg = data[ID_DATA_BACKGROUND];
		struct Jdata* dino = data[ID_PLAY_PLAYER];
		struct Jdata* debug = data[ID_PLAY_DEBUG];

		// Update score count here.
		game_state->score += game_state->point_mult;
		set_string(score, "SCORE: %d", game_state->score);
		render(score);

        // Update coin count
        if(game_state->coin_get){
            set_string(coin_t, "COINS: %d", game_state->coins);
            render(coin_t);
            game_state->coin_get = false;
        }

		// Update HiScore here
		if(game_state->score > game_state->hiscore){
			game_state->hiscore = game_state->score;
		    set_string(hiscore, "High Score: %d", game_state->hiscore);
		    render(hiscore);
		    hiscore->x = game_state->display_w - hiscore->data->w;
        }
		// Apply jump physics (up/down movement)

		switch(game_state->motion){
			case IDLE:
				break;
			case UP:
				if(dino->y > game_state->jump_height)
					dino->y = dino->y - (int)(game_state->game_speed * (30.0 / game_state->fps_limit));
				else
				  	game_state->motion = DOWN;
				break;
			case DOWN:
			  	if(dino->y < game_state->display_h - dino->data->h)
					dino->y = dino->y + (int)(game_state->game_speed * (30.0 / game_state->fps_limit));
			  	else if(dino->y >= game_state->display_h - dino->data->h)
				  	game_state->motion = IDLE;
			  	break;
		}
		
        if(bg->rect == NULL)
		    bg->rect = get_rect(bg, game_state);
		if(score->rect == NULL)
            score->rect = get_rect(score, game_state);
		hiscore->rect = get_rect(hiscore, game_state);
		if(dino->rect == NULL)
            dino->rect = get_rect(dino, game_state);
        if(coin_t->rect == NULL)
            coin_t->rect = get_rect(coin_t, game_state);

		// Blit the surfaces in order: bg, objects, score, player
		SDL_BlitSurface(bg->data, NULL, game_state->surface, bg->rect);

		// Update object position and generate new objects
		object_handler(data, game_state);		

		SDL_BlitSurface(score->data,   NULL, game_state->surface, score->rect);
		SDL_BlitSurface(hiscore->data, NULL, game_state->surface, hiscore->rect);
		SDL_BlitSurface(coin_t->data,  NULL, game_state->surface, coin_t->rect);
        SDL_BlitSurface(dino->data,    NULL, game_state->surface, dino->rect);
		
		// Blit debug overlay if enabled
		if(debug_overlay && ( game_state->render_tick != 0 ) ){
			double avgFPS = 1000000000.0 / (double)game_state->render_tick;

			// Update string
			set_string(debug, "FPS: %0.2f S: %d M: %d", avgFPS, game_state->game_speed, game_state->point_mult);
			render(debug);
			
            if(debug->rect == NULL)
			    debug->rect = get_rect(debug, game_state);

			SDL_BlitSurface(debug->data, NULL, game_state->surface, debug->rect);
		}
	}

	SDL_UpdateWindowSurface(game_state->window);
    
    // Frame render time
    game_state->render_tick = SDL_GetTicksNS() - game_state->start_tick;

    if(game_state->render_tick < (int)( 1000000000 / game_state->fps_limit ) ){
        unsigned int sleep = (1000000000 / game_state->fps_limit) - game_state->render_tick;
        SDL_DelayNS(sleep);
        
        game_state->render_tick = SDL_GetTicksNS() - game_state->start_tick;
    }
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

    if(!game_state->treasure[0] && ( rand()%500==0 ) ){
        game_state->treasure[0] = 1;
    }
	
    // Generate player rect
	struct Jdata* player = data[ID_PLAY_PLAYER];
	
    player->rect = get_rect(player, game_state);

    if(game_state->treasure[0]){
        
        struct Jdata* coin = data[ID_PLAY_COIN];
        unsigned long long int ns = SDL_GetTicksNS();

        if( (ns - game_state->animation_tick) > 83333333 ){
            game_state->animation_tick = ns;
        
            // Update all animation frames
            render(coin);
        }

        coin->x = coin->x - (int)(game_state->game_speed * (30.0 / game_state->fps_limit));

        coin->rect = get_rect(coin, game_state);

        if(check_collision(coin->rect, player->rect)){
            coin->x = -coin->data->w;
            game_state->coin_get = true;
            game_state->coins += 1;
        }

        if(coin->x < -coin->data->w){
            game_state->treasure[0] = 0;
            coin->x = game_state->display_w;
        }
    
        SDL_BlitSurface(coin->data, NULL, game_state->surface, coin->rect);
    }

    // Move and blit objects.
	for(int i = 0; i < game_state->objects; i++){
		struct Jdata* obj = data[ID_PLAY_OBJECT + i];
		if(obj != NULL && game_state->obstacle[i]){
			obj->y = game_state->display_h - obj->data->h;
			obj->x = obj->x - (int)(game_state->game_speed * (30.0 / game_state->fps_limit));

			obj->rect = get_rect(obj, game_state);

			// Detect collision and end play state if true
			if(check_collision(obj->rect, player->rect)){
				game_state->game_over = true;

				// Reset active objects
				for(int k = 0; k < 4; k++){
					game_state->obstacle[k] = 0;
				}

				if(game_state->score >= game_state->hiscore)
					write_score(game_state);
			}

			// Once object is off the screen we turn it off
			if(obj->x < -obj->data->w){
				obj->x = game_state->display_w;
				game_state->obstacle[i] = 0;
			}

			SDL_BlitSurface(obj->data, NULL, game_state->surface, obj->rect);
		}
	}
}

static bool check_collision(SDL_Rect* a, SDL_Rect* b){
	if(a->y + a->h <= b->y)
		return false;
	if(a->y >= b->y + b->h)
		return false;
	if(a->x + a->w <= b->x)
		return false;
	if(a->x >= b->x + b->w)
		return false;
	return true;
}

void read_score(Jgame* game_state){
	char value[9] = "00000000\0";
	FILE* f = jaccess("score", "r");
	jread(f, value, 8);
	fclose(f);
	SDL_sscanf(value, "%d", &game_state->hiscore);
	
	debug("Read score to be: %d", game_state->hiscore);
}

void write_score(Jgame* game_state){
	FILE* f = jaccess("score", "w");
	char content[64];
	
	SDL_snprintf(content, 64, "%d\n", game_state->score);
	jwrite(f, content);
	fclose(f);

	debug("Recording new hiscore: %d", game_state->score);
}

void handle_keys(SDL_KeyboardEvent e, Jgame* game_state){
	SDL_Keycode key = e.key;
	
	if(key == game_state->jump1 || key == game_state->jump2 || key == game_state->jump3){
	    if(game_state->motion == IDLE)
            game_state->motion = UP;
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		info("Toggle debug overlay to: %d", debug_overlay);
	}
}
