#include <stdbool.h>

struct Jtimer{
	// Store the starting tick count
	unsigned int startTicks;

	// Store the ticks when the timer was paused
	unsigned int pauseTicks;

	// Bools of timer state
	bool paused;
	bool running;
};

// Create timer
struct Jtimer* timer_init();

// Destroy timer
void timer_free(struct Jtimer*);

// Timer controls
void timer_start(struct Jtimer*);
void timer_stop(struct Jtimer*);
void timer_pause(struct Jtimer*);
void timer_unpause(struct Jtimer*);

// Retreive timer status
unsigned int timer_getTicks(struct Jtimer*);
bool timer_isCounting(struct Jtimer*);
bool timer_isPaused(struct Jtimer*);
