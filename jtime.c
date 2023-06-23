#ifdef WIN
#include <SDL.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#endif

#include "jtime.h"

struct Jtimer* timer_init(){
	struct Jtimer* tmr = malloc(sizeof(struct Jtimer));

	tmr->startTicks = 0;
	tmr->pauseTicks = 0;

	tmr->paused = false;
	tmr->running = false;

	return tmr;
}

void timer_start(struct Jtimer* tmr){
	tmr->running = true;
	tmr->paused = false;

	tmr->startTicks = SDL_GetTicks();
	tmr->pauseTicks = 0;
}

void timer_stop(struct Jtimer* tmr){
	tmr->running = false;
	tmr->paused = false;

	tmr->startTicks = 0;
	tmr->pauseTicks = 0;
}

void timer_pause(struct Jtimer* tmr){
	if(tmr->running && !tmr->paused){
		tmr->paused = true;

		tmr->pauseTicks = SDL_GetTicks() - tmr->startTicks;
		tmr->startTicks = 0;
	}
}

void timer_unpause(struct Jtimer* tmr){
	if(tmr->running && tmr->paused){
		tmr->paused = false;

		tmr->startTicks = SDL_GetTicks() - tmr->pauseTicks;

		tmr->pauseTicks = 0;
	}
}

unsigned int timer_getTicks(struct Jtimer* tmr){
	unsigned int time = 0;

	if(tmr->running){
		if(tmr->paused){
			time = tmr->pauseTicks;
		}else{
			time = SDL_GetTicks() - tmr->startTicks;
		}
	}

	return time;
}

bool timer_isCounting(struct Jtimer* tmr){
	return tmr->running;
}

bool timer_isPaused(struct Jtimer* tmr){
	return tmr->paused;
}

void timer_free(struct Jtimer* tmr){
	free(tmr);
}
