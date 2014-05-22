#include "window.h"

int n = 12;
int i = 0;

int Update_The_Surface(char * me)
{

    if (!start_game) //Look which player you are and make it blink
    {
        SDL_BlitSurface( XOut, NULL, ScreenSurface, NULL );
        //Waiting for more players
        if(strcmp(me, "player1") == 0)
        {
            if (n > 0)
            {
                SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
                n--;
                if(n == 0){i = 0;}
            }
            else if (i< 12 )
            {
                i++;
                if (i == 12){ n = 12; }

            }

            SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
            SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
            SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4); 
        }

        else if(strcmp(me, "player2") == 0)//Look which player you are and make it blink
        {
            if (n > 0)
            {
                SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
                n--;
                if(n == 0){i = 0;}
            }
            else if (i< 12 )
            {
                i++;
                if (i == 12){ n = 12; }

            }

            SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
            SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
            SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);
        }
        else if(strcmp(me, "player3") == 0)//Look which player you are and make it blink
        {
           if (n > 0)
            {
                SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
                n--;
                if(n == 0){i = 0;}
            }
            else if (i< 12 )
            {
                i++;
                if (i == 12){ n = 12; }

            }

            SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
            SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
            SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);
        }
        else if(strcmp(me, "player4") == 0)//Look which player you are and make it blink
        {
            if (n > 0)
            {
                SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);
                n--;
                if(n == 0){i = 0;}
            }
            else if (i< 12 )
            {
                i++;
                if (i == 12){ n = 12; }

            }

            SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
            SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
            SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
        }

        SDL_BlitSurface(text1, NULL, ScreenSurface, &rctext1);
        SDL_BlitSurface(text2, NULL, ScreenSurface, &rctext2);
        SDL_BlitSurface(text3, NULL, ScreenSurface, &rctext3);
        SDL_BlitSurface(text4, NULL, ScreenSurface, &rctext4);

    }
    else
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
        else if(strstr(wall_play, "play3"))
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
    }

    ///Update the surface
    //SDL_UpdateWindowSurface( Window );
    return 0;
}

int game_over_and_restart_the_game()
{
    start_game = false;
    for(i=0;i<=60;i++)
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

    ///Start position Player1
    rcPlayer1.x = SCREEN_WIDTH/2-rcPlayer1.w/2;   //Funkar bra rcPlayer1.x = SCREEN_WIDTH/2-100;
    rcPlayer1.y = SCREEN_HEIGHT-50;
    ///Start position Player2
    rcPlayer2.x = SCREEN_WIDTH/2-rcPlayer1.w/2;
    rcPlayer2.y = 25;
    ///Start position Player3
    rcPlayer3.x = 25;
    rcPlayer3.y = SCREEN_HEIGHT/2-rcPlayer1.w/2;
    ///Start position Player4
    rcPlayer4.x = SCREEN_WIDTH-50;
    rcPlayer4.y = SCREEN_HEIGHT/2-rcPlayer1.w/2;

    //Balls start position
    rcball.x = SCREEN_WIDTH/2-(rcball.w/2);
    rcball.y = SCREEN_HEIGHT/2-(rcball.h/2);

    text1 = TTF_RenderText_Blended(font3, score[points[1]], colors[points[1]]);
    text2 = TTF_RenderText_Blended(font3, score[points[2]], colors[points[2]]);
    text3 = TTF_RenderText_Blended_Wrapped(font3, score[points[3]], colors[points[3]],30);
    text4 = TTF_RenderText_Blended_Wrapped(font3, score[points[4]], colors[points[4]],30);

    n=12;
    i=0;
    return 0;
}
