#ifdef WIN
#include <SDL.h>
#include <SDL_ttf.h>
#endif

#ifdef NIX
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "jio.h"
#include "jdata.h"
#include "play.h"
#include "menu.h"
#include "jtime.h"

#define WINDOW_HEIGHT 1080/2
#define WINDOW_WIDTH 1920/2

enum Data {JIMAGE, JFONT};
enum State {MENU, PLAY, PAUSE, STORY, EXIT};
enum Position {CENTER = -1};
enum Targets {LMENU, LPLAY};

// For menu.c
enum Menu_Ids {ID_MENU_BACKGROUND, ID_MENU_PLAY, ID_MENU_STORY, ID_MENU_EXIT};

// For play.c
enum Play_Ids {ID_PLAY_BACKGROUND, ID_PLAY_PLAYER, ID_PLAY_3, ID_PLAY_2, ID_PLAY_1, ID_PLAY_GO, ID_PLAY_SCORE, ID_PLAY_HISCORE, ID_PLAY_OBJECT, ID_PLAY_OBJECT2, ID_PLAY_OBJECT3, ID_PLAY_OBJECT4};
