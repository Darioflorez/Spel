#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <enet/enet.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

///Screen dimension constants
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

extern bool GOAL;
extern bool FAILURE;
extern bool print_gameover;

///The window we'll be rendering to
extern SDL_Window *Window;
///The surface contained by the window
extern SDL_Surface *ScreenSurface;

///The image we will load and show on the screen
extern SDL_Surface* XOut;

//Score a point
extern SDL_Surface* scoreMade;
extern SDL_Rect rcscoreMade;


///The ball
extern SDL_Surface* Ball;
///Ball position
extern SDL_Rect rcball;

///All Players
extern SDL_Surface* Player1;
extern SDL_Surface* Player2;
extern SDL_Surface* Player3;
extern SDL_Surface* Player4;

extern SDL_Surface* Player1_text;
extern SDL_Surface* Player2_text;
extern SDL_Surface* Player3_text;
extern SDL_Surface* Player4_text;

extern SDL_Rect rcPlayer1;
extern SDL_Rect rcPlayer2;
extern SDL_Rect rcPlayer3;
extern SDL_Rect rcPlayer4;

extern SDL_Rect rcPlayer1_text;
extern SDL_Rect rcPlayer2_text;
extern SDL_Rect rcPlayer3_text;
extern SDL_Rect rcPlayer4_text;

//invisible players
extern SDL_Surface* InvisablePlayer1;
extern SDL_Surface* InvisablePlayer2;
extern SDL_Surface* InvisablePlayer3;
extern SDL_Surface* InvisablePlayer4;

//Wall
extern SDL_Surface* wall_p1;
extern SDL_Rect rcwall_p1;
extern char wall_play[20];
extern bool wall;

extern SDL_Surface* wall_p2;
extern SDL_Rect rcwall_p2;

extern SDL_Surface* wall_p3;
extern SDL_Rect rcwall_p3;

extern SDL_Surface* wall_p4;
extern SDL_Rect rcwall_p4;

//Score
extern SDL_Surface* text1;
extern SDL_Surface* text2;
extern SDL_Surface* text3;
extern SDL_Surface* text4;

extern SDL_Rect rctext1;
extern SDL_Rect rctext2;
extern SDL_Rect rctext3;
extern SDL_Rect rctext4;


extern int points[5];
extern int points_made;

///Font
extern TTF_Font* font;
extern TTF_Font* font2;
extern TTF_Font* font3;
extern TTF_Font* font4;

//end of game
extern SDL_Rect rcendofgame;
extern SDL_Surface* endofgame;

extern SDL_Color colors[1000];
extern SDL_Color color[2];
extern const char* score[1000];


///The Music
extern Mix_Chunk* effect;
extern Mix_Music* music;
extern double test;

extern bool create_window();
extern bool loadMedia();
extern void close();
extern int Update_The_Surface();
extern int mainMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int nextMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int instMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern bool loadMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern bool connect_to_server();
extern int game_over_and_restart_the_game();
#endif // WINDOW_H_INCLUDED
