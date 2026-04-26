#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdbool.h>

// TODO: Update flags, 

typedef struct{
    int x;
    int y;
} vec2;

typedef struct Jgame{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_AudioStream* audio_stream; // A sound stream to play audio events
    SDL_AudioStream* music_stream; // A sound stream to play music
    int display_w;
    int display_h;
    SDL_DisplayID* display_id; // A list of all displays 
    int n_displays; // How many displays are there

    struct Jdata** data_pack;

	// Settings
    int monitor; // The monitor we are using
    int volume; // Current game volume
    bool is_fullscreen; // Are we fullscreen
	bool is_vsync;
    int fps_limit;
    int game_speed;
    int point_mult;
    SDL_Keycode jump1;
	SDL_Keycode jump2;
	SDL_Keycode jump3;

    // Registers
    int ra, rb, rc, rd;

    // Timers
    unsigned long long int time_tick;
    unsigned long long int start_tick;
    unsigned long long int render_tick;
    unsigned long long int animation_tick;

	bool quit;

	// Gameplay
	int objects;
	int obstacle[10];
	int platform[10];
	int treasure[10];
	int motion; // Idle, Up, Down
	int score;
	int hiscore;
	int coins;
    bool coin_get;
    bool game_over;
	int jump_height;

} Jgame;

struct Jdata{
	// For all types
	// id - unique number of data package
	// type - enum of data type (image, font)
	// name - human readible name
	// path - path to the resource/image
	int id;
	int type;
	char* name;
	char* path;

    // Shared between JANIMATION and JFONT
    union{
        char* string;
        vec2 frames; 
    }aux;

	// For JFONT
	// text_bg - true/false to render background
	// fnt - loaded font package from path
	// fgColour - foreground text colour
	// bgColour - background text colour
	bool text_bg;
    TTF_Font* fnt;
    SDL_Color fgColour;
	SDL_Color bgColour;
	
    // SDL rendering data for JFONT JIMAGE JANIMATION
    // data - the data package, a rendered surface
    // rect - the SDL rect, define the location of the object
    union{
	    SDL_Surface* data;
        unsigned char* buffer;
    }data;

    union{
        SDL_Rect* rect;
        unsigned int length;
    }pram;
};


struct Jdata* init(int, int, char*, char*);
void render(struct Jdata*);
void play_sound(struct Jdata*, SDL_AudioStream*);
void stop_sound(SDL_AudioStream*);

void set_string(struct Jdata*, char*, ...);
void set_position(struct Jdata*, int, int, Jgame*);
void set_pos_x(struct Jdata*, int, Jgame*);
void set_pos_y(struct Jdata*, int, Jgame*);
void set_text_bg(struct Jdata*);
void set_fgColour(struct Jdata*, short int, short int, short int);
void set_bgColour(struct Jdata*, short int, short int, short int);
void set_font_size(struct Jdata*, int);

SDL_Rect* get_rect(struct Jdata*);
int get_pos_x(struct Jdata*);
int get_pos_y(struct Jdata*);
SDL_Surface* get_data(struct Jdata*);
SDL_Rect* get_rekt(struct Jdata*);

void jdata_free(struct Jdata*);
void jdata_print(struct Jdata*);
