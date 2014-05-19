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

    if (wall)
    {
        if(strstr(wall_play, "play1"))
        {
            SDL_BlitSurface(wall_p1, NULL, ScreenSurface, &rcwall_p1);
        }
        else if(strstr(wall_play, "play2"))
        {
            SDL_BlitSurface(wall_p2, NULL, ScreenSurface, &rcwall_p2);
        }
        else if(strstr(wall_play, "playr3"))
        {
            SDL_BlitSurface(wall_p3, NULL, ScreenSurface, &rcwall_p3);
        }
        else if(strstr(wall_play, "play4"))
        {
            SDL_BlitSurface(wall_p4, NULL, ScreenSurface, &rcwall_p4);
        }
        //strcpy(wall_play, "");
    }

    SDL_BlitSurface(text1, NULL, ScreenSurface, &rctext1);
    SDL_BlitSurface(text2, NULL, ScreenSurface, &rctext2);
    SDL_BlitSurface(text3, NULL, ScreenSurface, &rctext3);
    SDL_BlitSurface(text4, NULL, ScreenSurface, &rctext4);

    ///Update the surface
    //SDL_UpdateWindowSurface( Window );
    return 0;
}

int game_over_and_restart_the_game()
{
    for(int i=0;i<=60;i++)
    {
        font4 = TTF_OpenFont("good.ttf", i);
        rcendofgame.x = ScreenSurface->clip_rect.w/2-rcendofgame.w/2;
        rcendofgame.y = SCREEN_HEIGHT/2-30;
        endofgame = TTF_RenderText_Blended(font4,"GAME OVER!",color[0]);
        SDL_BlitSurface( XOut, NULL, ScreenSurface, NULL );
        SDL_BlitSurface(endofgame, NULL, ScreenSurface, &rcendofgame);
        SDL_UpdateWindowSurface( Window );
        SDL_Delay( 20 );
    }  
    SDL_Delay( 3000 );

    points[0]=0;
    points[1]=9;
    points[2]=9;
    points[3]=9;
    points[4]=9;
    points_made = 0;
    return 0;
}
