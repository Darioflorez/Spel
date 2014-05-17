#include "window.h"

int Update_The_Surface()
{
    ///Apply the image
    SDL_BlitSurface( XOut, NULL, ScreenSurface, NULL );
    SDL_BlitSurface(Ball, NULL, ScreenSurface, &rcball);

    if(points_made == 5 || points_made == 10 || points_made == 15 || 
        points_made == 20 || points_made == 25 || points_made == 30 || points_made == 35 || points_made == 40)
    {

        SDL_BlitSurface(InvisablePlayer1, NULL, ScreenSurface, &rcPlayer1);
        SDL_BlitSurface(InvisablePlayer2, NULL, ScreenSurface, &rcPlayer2);
        SDL_BlitSurface(InvisablePlayer3, NULL, ScreenSurface, &rcPlayer3);
        SDL_BlitSurface(InvisablePlayer4, NULL, ScreenSurface, &rcPlayer4); 
    }
    else
    {
        SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
        SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
        SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
        SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);
    }

    if (strstr(wall, "wall"))
    {
        if(strstr(wall, "play1"))
        {
            SDL_BlitSurface(wall_p1, NULL, ScreenSurface, &rcwall_p1);
        }
        else if(strstr(wall, "play2"))
        {
            SDL_BlitSurface(wall_p2, NULL, ScreenSurface, &rcwall_p2);
        }
        else if(strstr(wall, "playr3"))
        {
            SDL_BlitSurface(wall_p3, NULL, ScreenSurface, &rcwall_p3);
        }
        else if(strstr(wall, "play4"))
        {
            SDL_BlitSurface(wall_p4, NULL, ScreenSurface, &rcwall_p4);
        }
        strcpy(wall, "");
    }

    SDL_BlitSurface(text1, NULL, ScreenSurface, &rctext1);
    SDL_BlitSurface(text2, NULL, ScreenSurface, &rctext2);
    SDL_BlitSurface(text3, NULL, ScreenSurface, &rctext3);
    SDL_BlitSurface(text4, NULL, ScreenSurface, &rctext4);

    ///Update the surface
    //SDL_UpdateWindowSurface( Window );
    return 0;
}
