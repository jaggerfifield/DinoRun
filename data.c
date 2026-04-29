#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "main.h"
#include "jio.h"

struct Jdata* build(int, int, int, int, char*, char*, char*, Jgame*);

void loader(Jgame* game_state){
	info("data.c : Loading assets . . .");
    struct Jdata** data_pack = SDL_malloc(sizeof(struct Jdata*) * (ID_DATA_END+1));

    // Splash Screen
    debug("data.c : splash data loading");
    data_pack[ID_DATA_SPLASH] = build(ID_DATA_SPLASH, JIMAGE, -1, -1, "Splash Icon", "Assets/splash.bmp", NULL, game_state);
    
    SDL_BlitSurface(data_pack[ID_DATA_SPLASH]->data.data, NULL, game_state->surface, get_rect(data_pack[ID_DATA_SPLASH]));
    SDL_UpdateWindowSurface(game_state->window);

    data_pack[ID_DATA_BACKGROUND] = build(ID_DATA_BACKGROUND, JIMAGE, 0, 0, "Background texture", "Assets/bg_fill.bmp", NULL, game_state);
    data_pack[ID_DATA_GRID] = build(ID_DATA_GRID, JIMAGE, 0, 0, "Play Background", "Assets/grid.bmp", NULL, game_state);

	// Main Menu
    info("data.c : Loading menu data . . .");
    data_pack[ID_MAINMENU] = NULL;
	data_pack[ID_MAINMENU_PLAY] = build(ID_MAINMENU_PLAY, JFONT, CENTER, 0, "Play Text", "Assets/font.ttf", "Play", game_state);
	data_pack[ID_MAINMENU_ARCADE] = build(ID_MAINMENU_ARCADE, JFONT, CENTER, 100, "Arcade Text", "Assets/font.ttf", "Arcade", game_state);
	data_pack[ID_MAINMENU_SETTINGS] = build(ID_MAINMENU_SETTINGS, JFONT, CENTER, 200, "Settings Text", "Assets/font.ttf", "Settings", game_state);
	data_pack[ID_MAINMENU_EXIT] = build(ID_MAINMENU_EXIT, JFONT, CENTER, 300, "Exit Text", "Assets/font.ttf", "Exit", game_state);
	data_pack[ID_MAINMENU_TEST] = build(ID_MAINMENU_TEST, JANIMATION, 0, 0, "Test Animations", "Assets/chara/", NULL, game_state);

    // Play
    info("data.c : Loading play data . . .");
    data_pack[ID_PLAY] = NULL;
    data_pack[ID_PLAY_PLAYER] = build(ID_PLAY_PLAYER, JIMAGE, 25, game_state->display_h-500, "Player", "Assets/block.bmp", NULL, game_state);
    data_pack[ID_PLAY_TIMER] = build(ID_PLAY_TIMER, JFONT, CENTER, CENTER, "Countdown text", "Assets/font.ttf", "3", game_state);
    data_pack[ID_PLAY_SCORE] = build(ID_PLAY_SCORE, JFONT, 0, 0, "Score text", "Assets/font.ttf", "SCORE: 0", game_state);
    data_pack[ID_PLAY_HISCORE] = build(ID_PLAY_HISCORE, JFONT, 0, 0, "Highscore Text", "Assets/font.ttf", "HIGHSCORE: 0", game_state);
    data_pack[ID_PLAY_COINS] = build(ID_PLAY_COINS, JFONT, 0, 0, "Coins Text", "Assets/font.ttf", "COINS: 0", game_state);
    data_pack[ID_PLAY_OBJECT] = build(ID_PLAY_OBJECT, JIMAGE, game_state->display_w, 0, "Object 1", "Assets/dino.bmp", NULL, game_state);
    data_pack[ID_PLAY_OBJECT2] = build(ID_PLAY_OBJECT2, JIMAGE, game_state->display_w, 0, "Object 2", "Assets/ball.bmp", NULL, game_state);
    data_pack[ID_PLAY_OBJECT3] = build(ID_PLAY_OBJECT3, JIMAGE, game_state->display_w, 0, "Object 3", "Assets/OBJ3.bmp", NULL, game_state);
    data_pack[ID_PLAY_OBJECT4] = build(ID_PLAY_OBJECT4, JIMAGE, game_state->display_w, 0, "Object 4", "Assets/dino.bmp", NULL, game_state);
    data_pack[ID_PLAY_COIN] = build(ID_PLAY_COIN, JANIMATION, 0, 0, "Coin", "Assets/coin/", NULL, game_state);
    data_pack[ID_PLAY_DEBUG] = build(ID_PLAY_DEBUG, JFONT, 0, 0, "Debug overlay", "Assets/font.ttf", "DEBUG", game_state);

    // Gameover
    info("data.c : Loading gameover data . . .");
    data_pack[ID_GAMEOVER] = NULL;
    data_pack[ID_GAMEOVER_TEXT] = build(ID_GAMEOVER_TEXT, JFONT, CENTER, 100, "Game Over! text", "Assets/font.ttf", "Game Over!", game_state);
    data_pack[ID_GAMEOVER_PLAY] = build(ID_GAMEOVER_PLAY, JFONT, CENTER, CENTER, "Play again text", "Assets/font.ttf", "Retry", game_state);
    data_pack[ID_GAMEOVER_EXIT] = build(ID_GAMEOVER_EXIT, JFONT, CENTER, CENTER, "Quit text", "Assets/font.ttf", "Back", game_state);

    // Settings 
    info("data.c : Loading settings data . . .");
    data_pack[ID_SETTINGS] = NULL;
    data_pack[ID_SETTINGS_PAGE] = build(ID_SETTINGS_PAGE, JFONT, CENTER, 0, "Settings page", "Assets/font.ttf", "<  Settings Page  >", game_state);
    data_pack[ID_SETTINGS_1] = build(ID_SETTINGS_1, JFONT, CENTER, 100, "Settings page", "Assets/font.ttf", "<    >", game_state);
    data_pack[ID_SETTINGS_2] = build(ID_SETTINGS_2, JFONT, CENTER, 200, "Settings page", "Assets/font.ttf", "<    >", game_state);
    data_pack[ID_SETTINGS_3] = build(ID_SETTINGS_3, JFONT, CENTER, 300, "Settings page", "Assets/font.ttf", "<    >", game_state);
    data_pack[ID_SETTINGS_4] = build(ID_SETTINGS_4, JFONT, CENTER, 400, "Settings page", "Assets/font.ttf", "<    >", game_state);
    data_pack[ID_SETTINGS_5] = build(ID_SETTINGS_5, JFONT, CENTER, 500, "Settings page", "Assets/font.ttf", "<    >", game_state);
    data_pack[ID_SETTINGS_BACK] = build(ID_SETTINGS_BACK, JFONT, CENTER, 600, "Back", "Assets/font.ttf", "Back", game_state);
    
    // Sounds
    info("data.c : Loading sound data . . .");
    data_pack[ID_SOUND] = NULL;
    data_pack[ID_SOUND_JUMP] = init(ID_SOUND_JUMP, JSOUND, "Jumping sound", "Assets/sounds/jump.wav", game_state);
    data_pack[ID_SOUND_MENU] = init(ID_SOUND_MENU, JSOUND, "Menu sound", "Assets/sounds/menu.wav", game_state);
    data_pack[ID_SOUND_MUSIC] = init(ID_SOUND_MUSIC, JSOUND, "Background Music", "Assets/sounds/bg_music.wav", game_state);

    data_pack[ID_DATA_END] = NULL;

    game_state->data_pack = data_pack;
}

struct Jdata* build(int id, int type, int x, int y, char* title, char* path, char* string, Jgame* game_state){
    struct Jdata* node = init(id, type, title, path, game_state);

    if(string != NULL){
        set_string(node, string);
        render(node);
    }

    set_position(node, x, y, game_state);
    
    return node;
}

void free_data(Jgame* game_state){
    for(int i = ID_DATA_END; i>=0; i--){
		jdata_free(game_state->data_pack[i]);
		game_state->data_pack[i] = NULL;
    }
    
	SDL_free(game_state->data_pack);
	game_state->data_pack = NULL;
}
