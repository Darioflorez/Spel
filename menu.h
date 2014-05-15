#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>

extern int mainMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);

extern int nextMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);

extern int instMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);

extern bool loadMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);

#endif // MENU_H_INCLUDED
