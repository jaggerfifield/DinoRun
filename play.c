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
bool check_collision(SDL_FRect*, SDL_FRect*);
void read_score(Jgame*);
void write_score(Jgame*);

enum {IDLE, UP, DOWN};

// ===== game_state registers =====
// ra --> is the music playing?
// rb --> Current area we are playing
// rc --> nothing
// rd --> Frame Update Type

void play_state(Jgame* game_state){
	game_state->score = 1;
	game_state->game_over = false;
	game_state->objects = 4;
    game_state->motion = IDLE;
    game_state->point_mult = 1;
    game_state->coins = 0;
    game_state->coin_get = true; 

    game_state->ra = 0;
    game_state->rd = UPDATE_FRAME; // We want to update often since we are playing

    instance(game_state->trees, game_state->data_pack[ID_SPRITE_TREE], 10, game_state);
    
    for(int i = 0; i < 10; i++){
        set_pos_x(game_state->trees[i], (game_state->display_w/8)*i - (game_state->trees[i]->texture->w/2), game_state);
        set_pos_y(game_state->trees[i], game_state->display_h-game_state->trees[i]->texture->h, game_state);
    }

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
    set_pos_y(DTA[ID_PLAY_COINS], DTA[ID_PLAY_SCORE]->texture->h+10, game_state);
    
    int disp_w = game_state->display_w;
    set_pos_x(DTA[ID_PLAY_OBJECT], disp_w, game_state);
    set_pos_x(DTA[ID_PLAY_OBJECT2], disp_w, game_state);
    set_pos_x(DTA[ID_PLAY_OBJECT3], disp_w, game_state);
    set_pos_x(DTA[ID_PLAY_OBJECT4], disp_w, game_state);

	set_pos_y(DTA[ID_PLAY_PLAYER], game_state->display_h-DTA[ID_PLAY_PLAYER]->texture->h, game_state);
    
    set_pos_x(DTA[ID_PLAY_COIN], disp_w, game_state);
    set_pos_y(DTA[ID_PLAY_COIN], game_state->display_h-DTA[ID_PLAY_PLAYER]->texture->h-DTA[ID_PLAY_COIN]->texture->h-50, game_state);

	game_state->jump_height = game_state->display_h - DTA[ID_PLAY_PLAYER]->texture->h - 280;
	
    set_string(DTA[ID_PLAY_HISCORE], "High Score: %d", game_state->hiscore);
    draw_font(DTA[ID_PLAY_HISCORE], game_state);
	set_pos_x(DTA[ID_PLAY_HISCORE], disp_w - DTA[ID_PLAY_HISCORE]->texture->w, game_state);

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
	
    stop_sound(game_state->music_stream);

	if(!game_state->quit)
		gameover_state(game_state);
}

static void _update(Jgame* game_state, struct Jdata** data){
    if(SDL_GetTicksNS() - game_state->animation_tick > ANIMATION_TIME){
        game_state->animation_tick = SDL_GetTicksNS();
        game_state->rd = ANIMATION_FRAME;
    }

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
            draw_font(timer_node, game_state);

            render(game_state->data_pack[ID_DATA_BACKGROUND], game_state);
            render(timer_node, game_state);
        }
	// This is the main update, where we hande the gameplay
	}else{
        if(!game_state->ra){
            play_sound(game_state->data_pack[ID_SOUND_MUSIC], game_state->music_stream);
            game_state->ra = 1;
        }
		
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
        draw_font(score, game_state);

        // Update coin count
        if(game_state->coin_get){
            set_string(coin_t, "COINS: %d", game_state->coins);
            draw_font(coin_t, game_state);
            game_state->coin_get = false;
        }

		// Update HiScore here
		if(game_state->score > game_state->hiscore){
			game_state->hiscore = game_state->score;
		    set_string(hiscore, "High Score: %d", game_state->hiscore);
            draw_font(hiscore, game_state);
		    set_pos_x(hiscore, game_state->display_w - hiscore->texture->w, game_state);
        }
		// Apply jump physics (up/down movement)
        int dino_y = get_pos_y(dino);

		switch(game_state->motion){
			case IDLE:
				break;
			case UP:
				if(dino_y > game_state->jump_height)
					set_pos_y(dino, dino_y - (int)(game_state->game_speed * (30.0 / game_state->fps_limit)), game_state);
				else
				  	game_state->motion = DOWN;
				break;
			case DOWN:
			  	if(dino_y < game_state->display_h - dino->texture->h)
					set_pos_y(dino, dino_y + (int)(game_state->game_speed * (30.0 / game_state->fps_limit)), game_state);
			  	else if(dino_y >= game_state->display_h - dino->texture->h)
				  	game_state->motion = IDLE;
			  	break;
		}
		
		// Blit the surfaces in order: bg, objects, score, player
        render(bg, game_state);

        for(int i = 0; i < 10; i++){
            render(game_state->trees[i], game_state);
        }

		// Update object position and generate new objects
		object_handler(data, game_state);		

        render(score, game_state);
        render(hiscore, game_state);
        render(coin_t, game_state);
        render(dino, game_state);

		// Blit debug overlay if enabled
		if(debug_overlay && ( game_state->render_tick != 0 ) ){
			double avgFPS = 1000000000.0 / (double)game_state->render_tick;

			// Update string
			set_string(debug, "FPS: %0.2f S: %d M: %d", avgFPS, game_state->game_speed, game_state->point_mult);
			draw_font(debug, game_state);
            render(debug, game_state);
		}
	}

	//SDL_UpdateWindowSurface(game_state->window);
    SDL_RenderPresent(game_state->renderer);
    
    // Frame render time
    game_state->render_tick = SDL_GetTicksNS() - game_state->start_tick;

    if(game_state->render_tick < (int)( 1000000000 / game_state->fps_limit ) ){
        unsigned int sleep = (1000000000 / game_state->fps_limit) - game_state->render_tick;
        SDL_DelayNS(sleep);
        
        game_state->render_tick = SDL_GetTicksNS() - game_state->start_tick;
    }

    game_state->rd = UPDATE_FRAME; // Update often since we are playing
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
	
    if(game_state->treasure[0]){
        
        struct Jdata* coin = data[ID_PLAY_COIN];

        if(game_state->rd == ANIMATION_FRAME)
            next_frame(coin);

        set_pos_x(coin, get_pos_x(coin) - (int)(game_state->game_speed * (30.0 / game_state->fps_limit)), game_state);

        if(check_collision(coin->rect, player->rect)){
            set_pos_x(coin, -coin->texture->w, game_state);
            game_state->coin_get = true;
            game_state->coins += 1;
        }

        if(get_pos_x(coin) < -coin->texture->w){
            game_state->treasure[0] = 0;
            set_pos_x(coin, game_state->display_w, game_state);
        }
    
        render(coin, game_state);
    }

    // Move and blit objects.
	for(int i = 0; i < game_state->objects; i++){
		struct Jdata* obj = data[ID_PLAY_OBJECT + i];
		if(obj != NULL && game_state->obstacle[i]){
			set_pos_y(obj, game_state->display_h - obj->texture->h, game_state);
			set_pos_x(obj, get_pos_x(obj) - (int)(game_state->game_speed * (30.0 / game_state->fps_limit)), game_state);

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
			if(get_pos_x(obj) < -obj->texture->w){
				game_state->obstacle[i] = 0;
				set_pos_x(obj, game_state->display_w, game_state);
			}

            render(obj, game_state);
		}
	}
}

bool check_collision(SDL_FRect* a, SDL_FRect* b){
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
	    if(game_state->motion == IDLE){
            play_sound(game_state->data_pack[ID_SOUND_JUMP], game_state->audio_stream);
            game_state->motion = UP;
        }
	}else if(key == SDLK_F3){
		debug_overlay = ! debug_overlay;
		info("Toggle debug overlay to: %d", debug_overlay);
	}
}
