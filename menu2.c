#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "window.h"
#define MainNum 3
#define NextNum 2

int mainMenu(SDL_Window* Window, SDL_Surface* ScreenSurface, TTF_Font* font, Mix_Chunk* effect)
{
    int i;
    const char* labels[] = {"Start","Instructions","Exit"};
    bool selected[] = {1,0,0};
    SDL_Rect position[MainNum];
    SDL_Surface* Menu[MainNum];
    SDL_Color color[2] = {{255,255,255},{255,0,0}};
    Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
    Menu[1] = TTF_RenderText_Solid(font,labels[1],color[0]);
    Menu[2] = TTF_RenderText_Solid(font,labels[2],color[0]);
    position[0].x = 640/2;
    position[0].y = (480-100)/2;
    position[1].x = 640/2;
    position[1].y = (480-50)/2;
    position[2].x = 640/2;
    position[2].y = 480/2;

    //SDL_FillRect(ScreenSurface,NULL,0x00); ///Clear screen
    SDL_BlitSurface( Menu_Image, NULL, ScreenSurface, NULL );
    SDL_Event event;
    while(1)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                ///Close button clicked
                case SDL_QUIT:
                    for(i=0;i<MainNum;i++)
                    {
                        SDL_FreeSurface(Menu[i]);
                    }
                    return 2; //Close the window
                ///Handle the keyboard
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==(MainNum-1))
                                    {
                                        selected[0] = 1;
                                        SDL_FreeSurface(Menu[0]);
                                        Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
                                    }
                                    else
                                    {
                                        selected[i+1] = 1;
                                        SDL_FreeSurface(Menu[i+1]);
                                        Menu[i+1] = TTF_RenderText_Solid(font,labels[i+1],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_UP:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==0)
                                    {
                                        selected[MainNum-1] = 1;
                                        SDL_FreeSurface(Menu[MainNum-1]);
                                        Menu[MainNum-1] = TTF_RenderText_Solid(font,labels[MainNum-1],color[1]);
                                    }
                                    else
                                    {
                                        selected[i-1] = 1;
                                        SDL_FreeSurface(Menu[i-1]);
                                        Menu[i-1] = TTF_RenderText_Solid(font,labels[i-1],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_RETURN:
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    SDL_FreeSurface(Menu[i]);
                                    if(i==0) ///start
                                    {
                                        if (connect_to_server() == FAILURE) //Connect
                                        {
                                            return 1;
                                        }
                                        else
                                        {
                                            return 0;
                                        }
                                    }
                                    else if(i==(MainNum-2)) ///instructons
                                    {
                                        return 3;
                                    }
                                    else if(i==(MainNum-1)) ///exit
                                    {
                                        return 2;
                                    }
                                }
                            }
                        case SDLK_ESCAPE:
                            for(i=0;i<MainNum;i++)
                            {
                                SDL_FreeSurface(Menu[i]);
                            }
                            return 1;
                    }
                    break;
            }
        }
        for(i=0;i<MainNum;i++)
        {
            SDL_BlitSurface(Menu[i],NULL,ScreenSurface,&position[i]);
        }
        SDL_UpdateWindowSurface( Window );
    }
    return 0;
}


int nextMenu(SDL_Window* Window, SDL_Surface* ScreenSurface, TTF_Font* font, Mix_Chunk* effect)
{
    int i;
    const char* labels[] = {"4 computers","2 computers","1 computer"};
    bool selected[] = {1,0,0};
    SDL_Rect position[MainNum];
    SDL_Surface* Menu[MainNum];
    SDL_Color color[2] = {{255,255,255},{255,0,0}};
    Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
    Menu[1] = TTF_RenderText_Solid(font,labels[1],color[0]);
    Menu[2] = TTF_RenderText_Solid(font,labels[2],color[0]);
    position[0].x = 640/2;
    position[0].y = (480-100)/2;
    position[1].x = 640/2;
    position[1].y = (480-50)/2;
    position[2].x = 640/2;
    position[2].y = 480/2;

    //SDL_FillRect(ScreenSurface,NULL,0x00); ///Clear Screen
    SDL_BlitSurface( OPT_Image, NULL, ScreenSurface, NULL );
    SDL_Event event;
    while(1)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                ///Close button clicked
                case SDL_QUIT:
                    for(i=0;i<MainNum;i++)
                    {
                        SDL_FreeSurface(Menu[i]);
                    }
                    return 2; //Close the window
                ///Handle the keyboard
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==(MainNum-1))
                                    {
                                        selected[0] = 1;
                                        SDL_FreeSurface(Menu[0]);
                                        Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
                                    }
                                    else
                                    {
                                        selected[i+1] = 1;
                                        SDL_FreeSurface(Menu[i+1]);
                                        Menu[i+1] = TTF_RenderText_Solid(font,labels[i+1],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_UP:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==0)
                                    {
                                        selected[MainNum-1] = 1;
                                        SDL_FreeSurface(Menu[MainNum-1]);
                                        Menu[MainNum-1] = TTF_RenderText_Solid(font,labels[MainNum-1],color[1]);
                                    }
                                    else
                                    {
                                        selected[i-1] = 1;
                                        SDL_FreeSurface(Menu[i-1]);
                                        Menu[i-1] = TTF_RenderText_Solid(font,labels[i-1],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_RETURN:
                            for(i=0;i<MainNum;i++)
                            {
                                if(selected[i])
                                {
                                    SDL_FreeSurface(Menu[i]);
                                    return 0;
                                }

                            }
                        case SDLK_ESCAPE:
                            for(i=0;i<MainNum;i++)
                            {
                                SDL_FreeSurface(Menu[i]);
                            }
                            return 2; //Close the window
                    }
                    break;
            }
        }
        for(i=0;i<MainNum;i++)
        {
            SDL_BlitSurface(Menu[i],NULL,ScreenSurface,&position[i]);
        }
        SDL_UpdateWindowSurface( Window );
    }
    return 0;
}

int instMenu(SDL_Window* Window, SDL_Surface* ScreenSurface, TTF_Font* font, Mix_Chunk* effect)
{
    int i;
    const char* labels[] = {"New game","Exit"};
    const char* instruct[] = {"Player One: [Left] | [Right]","Player Two: [Left] | [Right]","Player Three: [Down] | [Up]","Player Four: [Down] | [Up]"};
    bool selected[] = {1,0};
    TTF_Font* font2 = TTF_OpenFont("Jura-Regular.ttf", 20);
    SDL_Rect position[NextNum];
    SDL_Rect posInstruct[4];
    SDL_Surface* Menu[NextNum];
    SDL_Surface* Info[4];
    SDL_Color color[] = {{255,255,255},{255,0,0}};
    Info[0] = TTF_RenderText_Solid(font2,instruct[0],color[0]);
    Info[1] = TTF_RenderText_Solid(font2,instruct[1],color[0]);
    Info[2] = TTF_RenderText_Solid(font2,instruct[2],color[0]);
    Info[3] = TTF_RenderText_Solid(font2,instruct[3],color[0]);
    Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
    Menu[1] = TTF_RenderText_Solid(font,labels[1],color[0]);
    position[0].x = 640/2;
    position[0].y = (480-100)/2;
    position[1].x = 640/2;
    position[1].y = (480-50)/2;
    posInstruct[0].x = 20;
    posInstruct[0].y = (500-150)/2;
    posInstruct[1].x = 20;
    posInstruct[1].y = (500-100)/2;
    posInstruct[2].x = 20;
    posInstruct[2].y = (500-50)/2;
    posInstruct[3].x = 20;
    posInstruct[3].y = 500/2;

    //SDL_FillRect(ScreenSurface,NULL,0x00); ///Clear Screen
    SDL_BlitSurface( OPT_Image, NULL, ScreenSurface, NULL );
    SDL_Event event;
    while(1)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                ///Close button clicked
                case SDL_QUIT:
                    for(i=0;i<NextNum;i++)
                    {
                        SDL_FreeSurface(Menu[i]);
                    }
                    return 2; ///Close the window
                ///Handle the keyboard
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<NextNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==1)
                                    {
                                        selected[0] = 1;
                                        SDL_FreeSurface(Menu[0]);
                                        Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
                                    }
                                    else
                                    {
                                        selected[1] = 1;
                                        SDL_FreeSurface(Menu[1]);
                                        Menu[1] = TTF_RenderText_Solid(font,labels[1],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_UP:
                            Mix_PlayChannel(-1,effect,0);
                            for(i=0;i<NextNum;i++)
                            {
                                if(selected[i])
                                {
                                    selected[i] = 0;
                                    SDL_FreeSurface(Menu[i]);
                                    Menu[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                    if(i==0)
                                    {
                                        selected[1] = 1;
                                        SDL_FreeSurface(Menu[1]);
                                        Menu[1] = TTF_RenderText_Solid(font,labels[1],color[1]);
                                    }
                                    else
                                    {
                                        selected[0] = 1;
                                        SDL_FreeSurface(Menu[0]);
                                        Menu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
                                    }
                                    break;
                                }
                            }
                            break;
                        case SDLK_RETURN:
                            for(i=0;i<NextNum;i++)
                            {
                                if(selected[i])
                                {
                                    SDL_FreeSurface(Menu[i]);
                                    if(i==0)
                                    {
                                        if (connect_to_server() == FAILURE)
                                        {
                                            return 1;
                                        }
                                        else
                                        {

                                            return 0;
                                        }
                                    }
                                    else if(i==1) ///Exit
                                    {
                                        return 2;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        case SDLK_ESCAPE:
                            for(i=0;i<NextNum;i++)
                            {
                                SDL_FreeSurface(Menu[i]);
                            }
                            return 1;
                    }
                    break;
            }
        }
        for(i=0;i<NextNum;i++)
        {
            SDL_BlitSurface(Menu[i],NULL,ScreenSurface,&position[i]);
        }
        for(i=0;i<4;i++)
        {
            SDL_BlitSurface(Info[i],NULL,ScreenSurface,&posInstruct[i]);
        }
        //SDL_BlitSurface(Instructions, NULL, gScreenSurface, &posInstruct);
        SDL_UpdateWindowSurface( Window );
    }
   return 0;
}

int loadMenu(SDL_Window* Window, SDL_Surface* ScreenSurface, TTF_Font* font, Mix_Chunk* effect)
{
    bool success = true;
    int i = mainMenu(Window,ScreenSurface,font,effect);
    if(i==3) //Go to instructions
    {
        int j = instMenu(Window,ScreenSurface,font,effect);
        if(j==0)
        {
            return 0;   //0 success
        }
        else if(j==1)
        {
            return -1; //1 failure
        }
        else if(j==2)
        {
            return 2; //Close the window
        }
    }
    else if(i==1)
    {
        return -1;
    }
    else if (i==0)
    {
        return 0;
    }

}
