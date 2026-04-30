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
    int music_volume; // Volume for music
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
	struct Jdata* trees[10];
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

    char* string;
    int n_frames;
    int current_frame;

	// For JFONT
	// text_bg - true/false to render background
	// fnt - loaded font package from path
	// fgColour - foreground text colour
	// bgColour - background text colour
    bool text_bg;
    TTF_Font* fnt;
    SDL_Color fgColour;
	SDL_Color bgColour;
	
    unsigned char* buffer; // JSOUND buffer
    unsigned int length;   // Length of JSOUND buffer
    
    SDL_Texture* texture;
    SDL_Texture** textures;
    SDL_FRect* rect;
};


struct Jdata* init(int, int, char*, char*, Jgame*);
void instance(struct Jdata**, struct Jdata*, int, Jgame*);
void render(struct Jdata*, Jgame*);
void draw_font(struct Jdata*, Jgame*);
void next_frame(struct Jdata*);
void play_sound(struct Jdata*, SDL_AudioStream*);
void stop_sound(SDL_AudioStream*);

void set_string(struct Jdata*, char*, ...);
void set_position(struct Jdata*, float, float, Jgame*);
void set_pos_x(struct Jdata*, float, Jgame*);
void set_pos_y(struct Jdata*, float, Jgame*);
void set_text_bg(struct Jdata*);
void set_fgColour(struct Jdata*, short int, short int, short int);
void set_bgColour(struct Jdata*, short int, short int, short int);
void set_font_size(struct Jdata*, int);

SDL_FRect* get_rect(struct Jdata*);
float get_pos_x(struct Jdata*);
float get_pos_y(struct Jdata*);

void jdata_free(struct Jdata*);
void jdata_print(struct Jdata*);
