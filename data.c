#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "main.h"
#include "jio.h"

void loader(Jgame* game_state){
	info("data.c : Loading assets . . .");
    struct Jdata** data_pack = SDL_malloc(sizeof(struct Jdata*) * ID_DATA_END);

    // Splash Screen
    debug("data.c : splash data loading");
    data_pack[ID_DATA_SPLASH] = init(ID_DATA_SPLASH, JIMAGE, -1, -1, "Splash Icon", "Assets/splash.bmp", NULL);
    
    SDL_BlitSurface(data_pack[ID_DATA_SPLASH]->data, NULL, game_state->surface, get_rect(data_pack[ID_DATA_SPLASH], game_state));
    SDL_UpdateWindowSurface(game_state->window);

    data_pack[ID_DATA_BACKGROUND] = init(ID_DATA_BACKGROUND, JIMAGE, 0, 0, "Background texture", "Assets/bg_fill.bmp", NULL);
    data_pack[ID_DATA_GRID] = init(ID_DATA_GRID, JIMAGE, 0, 0, "Play Background", "Assets/grid.bmp", NULL);

	// Main Menu
    info("data.c : Loading menu data . . .");
    data_pack[ID_MAINMENU] = NULL;
	data_pack[ID_MAINMENU_PLAY] = init(ID_MAINMENU_PLAY, JFONT, CENTER, 0, "Play Text", "Assets/font.ttf", "Play");
	data_pack[ID_MAINMENU_ARCADE] = init(ID_MAINMENU_ARCADE, JFONT, CENTER, 100, "Arcade Text", "Assets/font.ttf", "Arcade");
	data_pack[ID_MAINMENU_SETTINGS] = init(ID_MAINMENU_SETTINGS, JFONT, CENTER, 200, "Settings Text", "Assets/font.ttf", "Settings");
	data_pack[ID_MAINMENU_EXIT] = init(ID_MAINMENU_EXIT, JFONT, CENTER, 300, "Exit Text", "Assets/font.ttf", "Exit");
	data_pack[ID_MAINMENU_TEST] = init(ID_MAINMENU_TEST, JANIMATION, 0, 0, "Test Animations", "Assets/chara/", NULL);

    // Play
    info("data.c : Loading play data . . .");
    data_pack[ID_PLAY] = NULL;
    data_pack[ID_PLAY_PLAYER] = init(ID_PLAY_PLAYER, JIMAGE, 25, game_state->display_h-500, "Player", "Assets/block.bmp", NULL);
    data_pack[ID_PLAY_TIMER] = init(ID_PLAY_TIMER, JFONT, CENTER, CENTER, "Countdown text", "Assets/font.ttf", "3");
    data_pack[ID_PLAY_SCORE] = init(ID_PLAY_SCORE, JFONT, 0, 0, "Score text", "Assets/font.ttf", "SCORE: 0");
    data_pack[ID_PLAY_HISCORE] = init(ID_PLAY_HISCORE, JFONT, 0, 0, "Highscore Text", "Assets/font.ttf", "HIGHSCORE: 0");
    data_pack[ID_PLAY_COINS] = init(ID_PLAY_COINS, JFONT, 0, 0, "Coins Text", "Assets/font.ttf", "COINS: 0");
    data_pack[ID_PLAY_OBJECT] = init(ID_PLAY_OBJECT, JIMAGE, game_state->display_w, 0, "Object 1", "Assets/dino.bmp", NULL);
    data_pack[ID_PLAY_OBJECT2] = init(ID_PLAY_OBJECT2, JIMAGE, game_state->display_w, 0, "Object 2", "Assets/ball.bmp", NULL);
    data_pack[ID_PLAY_OBJECT3] = init(ID_PLAY_OBJECT3, JIMAGE, game_state->display_w, 0, "Object 3", "Assets/OBJ3.bmp", NULL);
    data_pack[ID_PLAY_OBJECT4] = init(ID_PLAY_OBJECT4, JIMAGE, game_state->display_w, 0, "Object 4", "Assets/dino.bmp", NULL);
    data_pack[ID_PLAY_COIN] = init(ID_PLAY_COIN, JANIMATION, 0, 0, "Coin", "Assets/coin/", NULL);
    data_pack[ID_PLAY_DEBUG] = init(ID_PLAY_DEBUG, JFONT, 0, 0, "Debug overlay", "Assets/font.ttf", "");

    // Gameover
    info("data.c : Loading gameover data . . .");
    data_pack[ID_GAMEOVER] = NULL;
    data_pack[ID_GAMEOVER_TEXT] = init(ID_GAMEOVER_TEXT, JFONT, CENTER, 100, "Game Over! text", "Assets/font.ttf", "Game Over!");
    data_pack[ID_GAMEOVER_PLAY] = init(ID_GAMEOVER_PLAY, JFONT, CENTER, CENTER, "Play again text", "Assets/font.ttf", "Retry");
    data_pack[ID_GAMEOVER_EXIT] = init(ID_GAMEOVER_EXIT, JFONT, CENTER, CENTER, "Quit text", "Assets/font.ttf", "Back");

    // Settings 
    info("data.c : Loading settings data . . .");
    data_pack[ID_SETTINGS] = NULL;
    data_pack[ID_SETTINGS_VOLUME] = init(ID_SETTINGS_VOLUME, JFONT, CENTER, 0, "Volume", "Assets/font.ttf", "<  Volume ---%  >");
    data_pack[ID_SETTINGS_DISPLAY] = init(ID_SETTINGS_DISPLAY, JFONT, CENTER, 100, "Display", "Assets/font.ttf", "<  Display:  >");
    data_pack[ID_SETTINGS_RESOLUTION]= init(ID_SETTINGS_RESOLUTION, JFONT, CENTER, 200, "Resolution", "Assets/font.ttf", "<  Resolution ---- x ----  >");
    data_pack[ID_SETTINGS_THEME] = init(ID_SETTINGS_THEME, JFONT, CENTER, 300, "Theme", "Assets/font.ttf", "<  Theme: Default  >");
    data_pack[ID_SETTINGS_DIFFICULTY] = init(ID_SETTINGS_DIFFICULTY, JFONT, CENTER, 400, "Difficulty", "Assets/font.ttf", "<  Difficulty: Normal  >");
    data_pack[ID_SETTINGS_FULLSCREEN] = init(ID_SETTINGS_FULLSCREEN, JFONT, CENTER, 500, "Fullscreen", "Assets/font.ttf", "<  Fullscreen:  >");
    data_pack[ID_SETTINGS_VSYNC] = init(ID_SETTINGS_VSYNC, JFONT, CENTER, 600, "VSync", "Assets/font.ttf", "<  VSync :  >");
    data_pack[ID_SETTINGS_FPSLIMIT] = init(ID_SETTINGS_FPSLIMIT, JFONT, CENTER, 700, "FPS Limit", "Assets/font.ttf", "<  FPS Limit:  >");
    data_pack[ID_SETTINGS_BACK] = init(ID_SETTINGS_BACK, JFONT, CENTER, 800, "Back", "Assets/font.ttf", "Back");
    
    data_pack[ID_DATA_END] = NULL;

    game_state->data_pack = data_pack;
}

void free_data(Jgame* game_state){
    for(int i = ID_DATA_END; i>=0; i--){
		jdata_free(game_state->data_pack[i]);
		game_state->data_pack[i] = NULL;
    }
    
	game_state->data_pack = NULL;
}
