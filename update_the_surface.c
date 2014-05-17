#include "window.h"

int Update_The_Surface()
{
    ///Apply the image
    SDL_BlitSurface( XOut, NULL, ScreenSurface, NULL );
    SDL_BlitSurface(Ball, NULL, ScreenSurface, &rcball);
    SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
    SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
    SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
    SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);

    SDL_BlitSurface(text1, NULL, ScreenSurface, &rctext1);
    SDL_BlitSurface(text2, NULL, ScreenSurface, &rctext2);
    SDL_BlitSurface(text3, NULL, ScreenSurface, &rctext3);
    SDL_BlitSurface(text4, NULL, ScreenSurface, &rctext4);

    /*if(GOAL)
    {
        SDL_BlitSurface(scoreMade, NULL, ScreenSurface, &rcscoreMade);

    }*/

    ///Update the surface
    //SDL_UpdateWindowSurface( Window );
    return 0;
}
