#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

///Screen dimension constants
//extern int SCREEN_WIDTH;
//extern int SCREEN_HEIGHT;

///The window we'll be rendering to
extern SDL_Window *Window;
///The surface contained by the window
extern SDL_Surface *ScreenSurface;

///The image we will load and show on the screen
extern SDL_Surface* XOut;

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


extern bool create_window();
extern bool loadMedia();
extern void close();
extern int GameLoop();
extern int Update_The_Surface();
#endif // WINDOW_H_INCLUDED
