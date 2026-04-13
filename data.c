#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "main.h"
#include "update.h"
#include "jio.h"

Jgame* loader(Jgame* game_state){
	info("data.c : Loading assets . . .");
    game_state->data_pack = malloc(sizeof(struct Jdata**) * 6);

    // Splash Screen
    debug("data.c : splash data loading");
	struct Jdata* splash[2];
    splash[0] = init(0, JIMAGE, -1, -1, "Splash Icon", "Assets/splash.bmp", NULL);
	splash[1] = NULL;
    update(game_state, splash, NULL);
	jdata_free(splash[0]);


	// Main Menu
    info("data.c : Loading menu data . . .");
	struct Jdata** main_menu = malloc(sizeof(struct Jdata) * 7);
	main_menu[0] = init(ID_MAINMENU_BACKGROUND, JIMAGE, 0, 0, "Background Image", "Assets/bg_fill.bmp", NULL);
	main_menu[1] = init(ID_MAINMENU_PLAY, JFONT, CENTER, 0, "Play Text", "Assets/font.ttf", "Play");
	main_menu[2] = init(ID_MAINMENU_ARCADE, JFONT, CENTER, 100, "Arcade Text", "Assets/font.ttf", "Arcade");
	main_menu[3] = init(ID_MAINMENU_SETTINGS, JFONT, CENTER, 200, "Settings Text", "Assets/font.ttf", "Settings");
	main_menu[4] = init(ID_MAINMENU_EXIT, JFONT, CENTER, 300, "Exit Text", "Assets/font.ttf", "Exit");
	main_menu[5] = init(ID_MAINMENU_EXIT+1, JANIMATION, 0, 0, "Test Animations", "Assets/chara/", NULL);
	main_menu[6] = NULL;

	game_state->data_pack[0] = main_menu;

    // Play
    info("data.c : Loading play data . . .");
    struct Jdata** play_data = malloc(sizeof(struct Jdata) * 13);
    play_data[0] = init(ID_PLAY_BACKGROUND, JIMAGE, 0, 0, "Play Background", "Assets/bg_fill.bmp", NULL);
    play_data[1] = init(ID_PLAY_PLAYER, JIMAGE, 25, game_state->display_h-500, "Player", "Assets/block.bmp", NULL);
    play_data[2] = init(ID_PLAY_TIMER, JFONT, CENTER, CENTER, "Countdown text", "Assets/font.ttf", "3");
    play_data[3] = init(ID_PLAY_SCORE, JFONT, 0, 0, "Score text", "Assets/font.ttf", "SCORE: 0");
    play_data[4] = init(ID_PLAY_HISCORE, JFONT, 0, 0, "Highscore Text", "Assets/font.ttf", "HIGHSCORE: 0");
    play_data[5] = init(ID_PLAY_COINS, JFONT, 0, 0, "Coins Text", "Assets/font.ttf", "COINS: 0");
    play_data[6] = init(ID_PLAY_OBJECT, JIMAGE, game_state->display_w, 0, "Object 1", "Assets/dino.bmp", NULL);
    play_data[7] = init(ID_PLAY_OBJECT2, JIMAGE, game_state->display_w, 0, "Object 2", "Assets/ball.bmp", NULL);
    play_data[8] = init(ID_PLAY_OBJECT3, JIMAGE, game_state->display_w, 0, "Object 3", "Assets/OBJ3.bmp", NULL);
    play_data[9] = init(ID_PLAY_OBJECT4, JIMAGE, game_state->display_w, 0, "Object 4", "Assets/dino.bmp", NULL);
    play_data[10] = init(ID_PLAY_COIN, JANIMATION, 0, 0, "Coin", "Assets/coin/", NULL);
    play_data[11] = init(ID_PLAY_DEBUG, JFONT, 0, 0, "Debug overlay", "Assets/font.ttf", "");
    play_data[12] = NULL;

    game_state->data_pack[1] = play_data;

    // Gameover
    info("data.c : Loading gameover data . . .");
    struct Jdata** gameover_data = malloc(sizeof(struct Jdata)* 4);
    gameover_data[0] = init(ID_GAMEOVER_TEXT, JFONT, CENTER, 100, "Game Over! text", "Assets/font.ttf", "Game Over!");
    gameover_data[1] = init(ID_GAMEOVER_PLAY, JFONT, CENTER, CENTER, "Play again text", "Assets/font.ttf", "Retry");
    gameover_data[2] = init(ID_GAMEOVER_EXIT, JFONT, CENTER, CENTER, "Quit text", "Assets/font.ttf", "Back");
    gameover_data[3] = NULL;

    game_state->data_pack[2] = gameover_data;

    // Settings 
    info("data.c : Loading settings data . . .");
    struct Jdata** settings_data = malloc(sizeof(struct Jdata)* 11);
    settings_data[0] = init(ID_SETTINGS_BACKGROUND, JIMAGE, 0, 0, "Menu Background", "Assets/bg_fill.bmp", NULL);
    settings_data[1] = init(ID_SETTINGS_VOLUME, JFONT, CENTER, 0, "Volume", "Assets/font.ttf", "<  Volume ---%  >");
    settings_data[2] = init(ID_SETTINGS_DISPLAY, JFONT, CENTER, 100, "Display", "Assets/font.ttf", "<  Display:  >");
    settings_data[3] = init(ID_SETTINGS_RESOLUTION, JFONT, CENTER, 200, "Resolution", "Assets/font.ttf", "<  Resolution ---- x ----  >");
    settings_data[4] = init(ID_SETTINGS_THEME, JFONT, CENTER, 300, "Theme", "Assets/font.ttf", "<  Theme: Default  >");
    settings_data[5] = init(ID_SETTINGS_DIFFICULTY, JFONT, CENTER, 400, "Difficulty", "Assets/font.ttf", "<  Difficulty: Normal  >");
    settings_data[6] = init(ID_SETTINGS_FULLSCREEN, JFONT, CENTER, 500, "Fullscreen", "Assets/font.ttf", "<  Fullscreen:  >");
    settings_data[7] = init(ID_SETTINGS_VSYNC, JFONT, CENTER, 600, "VSync", "Assets/font.ttf", "<  VSync :  >");
    settings_data[8] = init(ID_SETTINGS_FPSLIMIT, JFONT, CENTER, 700, "FPS Limit", "Assets/font.ttf", "<  FPS Limit:  >");
    settings_data[9] = init(ID_SETTINGS_BACK, JFONT, CENTER, 800, "Back", "Assets/font.ttf", "Back");
    settings_data[10] = NULL;

    game_state->data_pack[3] = settings_data;

    // Story
    info("data.c : Loading Sotry data . . .");
    struct Jdata** story_data = malloc(sizeof(struct Jdata)*4);
    story_data[0] = init(ID_STORY_BACKGROUND, JIMAGE, 0, 0, "Play Background", "Assets/bg_fill.bmp", NULL);
    story_data[1] = init(ID_STORY_PLAYER, JIMAGE, 25, 100, "Player", "Assets/image.bmp", NULL);
    story_data[2] = init(ID_STORY_DEBUG, JFONT, 0, 0, "Debug Overlay", "Assets/font.ttf", "");
    story_data[3] = NULL;

    game_state->data_pack[4] = story_data;

    game_state->data_pack[5] = NULL;
    return game_state;
}

void free_data(Jgame* game_state){
    for(int h = 0; game_state->data_pack[h] != NULL; h++){
	    struct Jdata** node = game_state->data_pack[h];
            
        int i = 0;

        while(node[i] != NULL){
		    jdata_free(node[i]);
		    node[i] = NULL;
		    i = i + 1;
	    }

		free(node);
		node = NULL;
    }
    
	free(game_state->data_pack);
	game_state->data_pack = NULL;
}
