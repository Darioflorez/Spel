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

extern SDL_Rect rcPlayer1;
extern SDL_Rect rcPlayer2;
extern SDL_Rect rcPlayer3;
extern SDL_Rect rcPlayer4;

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

///Font
extern TTF_Font* font;
extern TTF_Font* font2;
extern TTF_Font* font3;
extern TTF_Font* font4;

extern SDL_Color colors[1000];
extern SDL_Color color[2];
extern const char* score[1000];

extern bool create_window();
extern bool loadMedia();
extern void close();
extern int GameLoop();
extern int Update_The_Surface();
#endif // WINDOW_H_INCLUDED
