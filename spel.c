
//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "menu.h"
#define scoreMax 10

///Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double    PI = 3.14159265;
const int FRAME_PER_SECOND = 40;
int Intervall;
///Time controll
int NextTick;

///Starts up SDL and creates window
bool init();

///Loads media
bool loadMedia();

///Frees media and shuts down SDL
void close();

///The window we'll be rendering to
SDL_Window* gWindow = NULL;

///The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

///The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

///The ball
SDL_Surface* Ball = NULL;
///Ball position
SDL_Rect rcball;

///Ball direction
double DirectionX, DirectionY;

///The Player1
SDL_Surface* Player1;
///Player1 position
SDL_Rect rcPlayer1;

///The Player2
SDL_Surface* Player2;
///Player2 position
SDL_Rect rcPlayer2;

///The Player3
SDL_Surface* Player3;
///Player3 position
SDL_Rect rcPlayer3;

///The Player4
SDL_Surface* Player4;
///Player4 position
SDL_Rect rcPlayer4;

SDL_Surface* InvisablePlayer1;
SDL_Surface* InvisablePlayer2;
SDL_Surface* InvisablePlayer3;
SDL_Surface* InvisablePlayer4;

SDL_Surface* wall_p1;
SDL_Rect rcwall_p1;

SDL_Surface* wall_p2;
SDL_Rect rcwall_p2;

SDL_Surface* wall_p3;
SDL_Rect rcwall_p3;

SDL_Surface* wall_p4;
SDL_Rect rcwall_p4;

void resetPlayerPosition(){
  ///Start position Player1
  rcPlayer1.x = 640/2-75;
  rcPlayer1.y = 480-50;
  ///Start position Player2
  rcPlayer2.x = 640/2-75;
  rcPlayer2.y = 25;
  ///Start position Player3
  rcPlayer3.x = 25;
  rcPlayer3.y = 480/2-75;
  ///Start position Player4
  rcPlayer4.x = 640-50;
  rcPlayer4.y = 480/2-75;
}




double angleEffect(struct SDL_Rect ball, struct SDL_Rect player, int playernum)
{
  int ball_x,ball_y;

  if(playernum == 1 || playernum == 2)
  {
    ball_x = ball.x + ball.w/2;

    if(ball_x > (player.x + 50) && ball_x < (player.x + 100)){/// MITTEN 50 pixlar
      return 0;
    }
    else if(ball_x > (player.x + 25) && ball_x < (player.x + 50)){/// vänster mitt
      return -15;
    }
    else if(ball_x > (player.x + 100) && ball_x < (player.x + 125)){/// höger mitt
      return 15;
    }
    else if( ball_x < (player.x + 25) ){ /// längst till vänster
      return -30;
    }
    else if( ball_x > (player.x + 125) ){ ///högersida
      return 30;
    }
  }
  else
  {
    ball_y = ball.y + ball.h/2;
  
    if(ball_y > (player.y + 50) && ball_y < (player.y + 100)){/// MITTEN 50 pixlar
      return 0;
    }
    else if(ball_y > (player.y + 25) && ball_y < (player.y + 50)){/// vänster mitt
      return 15;
    }
    else if(ball_y > (player.y + 100) && ball_y < (player.y + 125)){/// höger mitt
      return -15;
    }
    else if( ball_y < (player.y + 25) ){ /// längst till vänster
      return 30;
    }
    else if( ball_y > (player.y + 125) ){ ///högersida
      return -30;
    }
  }

  puts("INGEN EFFEKT!");
  return 0;
}

double distance( int x1, int y1, int x2, int y2 ){

	return sqrt( pow(x2-x1,2) + pow(y2-y1,2) );

}


bool Collition(struct SDL_Rect player, struct SDL_Rect ball){
	int px, py,i;
	int ball_x = ball.x + ball.w/2;/// Ball mid X
	int ball_y = ball.y + ball.h/2;/// Ball mid Y
	int ball_r = ball.w/2;
	double dis, dis_y, dis_x;

	// Check closest box x from the ball
	for(i=0;i<ball.w;i++)
	{
		if( ball_x < (player.x) ){
			px = player.x;
		}
		else if( ball_x > (player.x + player.w) ){
			px = (player.x + player.w);
		}
		else{
			px = ball.x + i;
		}
		//check closest box y from the ball
		if( ball_y < player.y ){
			py = player.y;
		}
		else if( ball_y > (player.y + player.h) ){
			py = (player.y + player.h);
		}
		else{
			py = ball.y + i;
		}

		if( distance(ball_x,ball_y,px,py) < ball_r){
			return true;
		}
	}	
	return false;
}

///EFFECTS
bool invisable = false;
bool visable = true;
int points_made = 0; /// points made in the game
int invisable_effekt = 0; /// Number on player who gets the effekt


///Ball random
double acc_vel = 5;
double angle = 0, angle2=0;
double scale_x= 0, scale_y=0;
double Resultante=0;
double Vel_x=0, Vel_y=0;
int acc_distance = 0;
///Make the ball move smoothly
int StartPosition_x=0, StartPosition_y=0;

///Restart ball
void newDirectionBall(double angle, struct SDL_Rect &ball)
{
    acc_distance = 0;
    printf("\nRCBALL.X: %d ", ball.x);
    printf("\nRCBALL.Y: %d ", ball.y);
    scale_x = cos((PI*angle)/180);
    scale_y = (sin((PI*angle)/180))*(-1);
    Vel_x = scale_x * acc_vel;
    Vel_y = scale_y * acc_vel;
    StartPosition_x = ball.x;
    StartPosition_y = ball.y;
    Resultante = sqrt((scale_x*scale_x)+(scale_y*scale_y));
    printf("\nANGLE: %f ", angle);
    printf("\nSIN: %f ", scale_y);
    printf("\nCOS: %f ", scale_x);
    printf("\nRESULANTE: %f ", Resultante);
    acc_vel += 0.1;
}

void RestartBall(struct SDL_Rect &ball)/// accumelated velosity
{
  acc_vel = 5;
  acc_distance = 0;
  ball.x = SCREEN_WIDTH/2-30;
  ball.y = SCREEN_HEIGHT/2-30;
}


void MoveBall(struct SDL_Rect &ball)
{
        ball.x = StartPosition_x + (Vel_x + (Vel_x * acc_distance));
        ball.y = StartPosition_y + (Vel_y + (Vel_y * acc_distance));
        acc_distance++;
}

void FPS_Init()
{
    NextTick = 0;
    Intervall = 1*1000/FRAME_PER_SECOND;
    return;
}

void FPS_Fn()
{
    if(NextTick > SDL_GetTicks())
    {
        SDL_Delay(NextTick - SDL_GetTicks());
    }
    NextTick = SDL_GetTicks() + Intervall;
}

void checkEffects(int points, int player)
{ /// Points made in the game, which player point is made on
  if(points_made == 5){
    invisable = true;
    invisable_effekt = player;
  }
  else{
    invisable = false;
  }
}

///Score
SDL_Surface* text1;
SDL_Surface* text2;
SDL_Surface* text3;
SDL_Surface* text4;

SDL_Surface* scoreMade;
SDL_Surface* endofgame;

SDL_Surface* Player1_text;
SDL_Surface* Player2_text;
SDL_Surface* Player3_text;
SDL_Surface* Player4_text;

SDL_Color colors[] = {{255,0,0},{255,51,0},{255,102,0},{255,153,0},{255,204,0},{255,255,0},{204,255,0},{153,255,0},{102,255,0},{51,255,0},{0,255,0}};
//{{0,255,0},{51,255,0},{102,255,0},{153,255,0},{204,255,0},{255,255,0},{255,204,0},{255,153,0},{255,102,0},{255,51,0},{255,0,0}};
SDL_Rect rctext1;
SDL_Rect rctext2;
SDL_Rect rctext3;
SDL_Rect rctext4;

SDL_Rect rcscoreMade;
SDL_Rect rcendofgame;

SDL_Rect rcPlayer1_text;
SDL_Rect rcPlayer2_text;
SDL_Rect rcPlayer3_text;
SDL_Rect rcPlayer4_text;

SDL_Color color[2] = {{255,255,255},{255,0,0}};

///Font
TTF_Font* font = NULL;
TTF_Font* font2 = NULL;
TTF_Font* font3 = NULL;
TTF_Font* font4 = NULL;

const char* score[] = {" ","D","D D","D D D","D D D D","D D D D D","D D D D D D","D D D D D D D","D D D D D D D D","D D D D D D D D D","D D D D D D D D D D"};
//{"DDDDDDDDDD","DDDDDDDDD","DDDDDDDD","DDDDDDD","DDDDDD","DDDDD","DDDD","DDD","DD","D","Game Over"}; ///Hearts

///The Music
Mix_Chunk* effect;
Mix_Music* music;
double test = Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,768);

bool init()
{
	///Initialization flag
	bool success = true;

	///Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		///Create window
		gWindow = SDL_CreateWindow( "Ping-Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			///Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	///Initialize TTF
	if (TTF_Init() == -1)
	{
        return false;
	}

	return success;
}

/// Every players points, player1 at points[1], player2 at points[2] ....you get it
int points[5] = {0,9,9,9,9};

bool loadMedia()
{
	///Loading success flag
	bool success = true;

	///Load splash image
	gXOut = SDL_LoadBMP( "black.bmp" );
	if( gXOut == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "black.bmp", SDL_GetError() );
		success = false;
	}

	///Create a window surface
	/*gScreenSurface = SDL_GetWindowSurface(gWindow);
	///Fill the surface
	SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 50, 64, 64));
    */
  InvisablePlayer1 = SDL_LoadBMP("block_invisable_p1_p2.bmp");
  if(InvisablePlayer1 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "block_invisable_p1_p2.bmp", SDL_GetError() );
    success = false;
  }
  InvisablePlayer2 = SDL_LoadBMP("block_invisable_p1_p2.bmp");
  if(InvisablePlayer2 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "block_invisable_p1_p2.bmp", SDL_GetError() );
    success = false;
  }
  InvisablePlayer3 = SDL_LoadBMP("block_invisable_p3_p4.bmp");
  if(InvisablePlayer3 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "block_invisable_p3_p4.bmp", SDL_GetError() );
    success = false;
  }
  InvisablePlayer4 = SDL_LoadBMP("block_invisable_p3_p4.bmp");
  if(InvisablePlayer4 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "block_invisable_p3_p4.bmp", SDL_GetError() );
    success = false;
  }

  wall_p1 = SDL_LoadBMP("wall_p1.bmp");
  if(wall_p1 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "wall_p1.bmp", SDL_GetError() );
    success = false;
  }

  wall_p2 = SDL_LoadBMP("wall_p2.bmp");
  if(wall_p2 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "wall_p2.bmp", SDL_GetError() );
    success = false;
  }

    wall_p3 = SDL_LoadBMP("wall_p3.bmp");
  if(wall_p3 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "wall_p3.bmp", SDL_GetError() );
    success = false;
  }

    wall_p4 = SDL_LoadBMP("wall_p4.bmp");
  if(wall_p4 == NULL)
  {
    printf( "Unable to load image %s! SDL Error: %s\n", "wall_p4.bmp", SDL_GetError() );
    success = false;
  }

	///Load a ball
	Ball = SDL_LoadBMP("ball4.bmp");
	if(Ball == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "ball4.bmp", SDL_GetError() );
		success = false;
	}

	///Load a Player1
	Player1 = SDL_LoadBMP("block_p1_p2.bmp");
	if(Player1 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "block_p1.bmp", SDL_GetError() );
		success = false;
	}

	///Load a Player2
	Player2 = SDL_LoadBMP("block_p1_p2.bmp");
	if(Player2 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "rectangle_yellow2.bmp", SDL_GetError() );
		success = false;
	}

	Player3 = SDL_LoadBMP("block_p3_p4.bmp");
	if(Player3 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "rectangle_blue.bmp", SDL_GetError() );
		success = false;
	}

	///Load a Player2
	Player4 = SDL_LoadBMP("block_p3_p4.bmp");
	if(Player3 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "rectangle4.bmp", SDL_GetError() );
		success = false;
	}

    ///Load a Font
    font = TTF_OpenFont("good.ttf", 32);
    if(font == NULL)
    {
        printf( "Unable to load font %s! SDL Error: %s\n", "good.ttf", SDL_GetError() );
        success = false;
    }
    font2 = TTF_OpenFont("Jura-Regular.ttf", 20);
    if(font2 == NULL)
    {
        printf( "Unable to load font %s! SDL Error: %s\n", "Jura-Regular.ttf", SDL_GetError() );
        success = false;
    }
    font3 = TTF_OpenFont("Heart.ttf", 20);
    if(font3 == NULL)
    {
        printf( "Unable to load font %s! SDL Error: %s\n", "Heart.ttf", SDL_GetError() );
        success = false;
    }
    font4 = TTF_OpenFont("good.ttf", 60);
    if(font3 == NULL)
    {
        printf( "Unable to load font %s! SDL Error: %s\n", "good.ttf", SDL_GetError() );
        success = false;
    }
    ///Load a text
    text1 = TTF_RenderText_Blended(font3, score[points[1]], colors[points[1]]);
    if(text1 == NULL)
    {
        return -1;
    }
    text2 = TTF_RenderText_Blended(font3, score[points[2]], colors[points[2]]);
    if(text2 == NULL)
    {
        return -1;
    }
    text3 = TTF_RenderText_Blended_Wrapped(font3, score[points[3]], colors[points[3]],30);
    if(text3 == NULL)
    {
        return -1;
    }
    text4 = TTF_RenderText_Blended_Wrapped(font3, score[points[4]], colors[points[4]],30);
    if(text4 == NULL)
    {
        return -1;
    }

    scoreMade = TTF_RenderText_Blended(font4, "SCORE!", color[0]);
    if(scoreMade==NULL)
    {
        return -1;
    }

    Player1_text = TTF_RenderText_Blended(font2, "Player 1: ", color[0]);
    if(Player1_text == NULL)
    {
        return -1;
    }
    Player2_text = TTF_RenderText_Blended(font2, "Player 2: ", color[0]);
    if(Player2_text == NULL)
    {
        return -1;
    }
    Player3_text = TTF_RenderText_Blended_Wrapped(font2, "P l a y e r 3", color[0],15);
    if(Player3_text == NULL)
    {
        return -1;
    }
    Player4_text = TTF_RenderText_Blended_Wrapped(font2, "P l a y e r 4", color[0],15);
    if(Player4_text == NULL)
    {
        return -1;
    }
    rctext1.x = 640/2-100;
    rctext1.y = 480-20;
    rctext2.x = 640/2-100;
    rctext2.y = 0;
    rctext3.x = 0;
    rctext3.y = 480/2-100;
    rctext4.x = 640-20;
    rctext4.y = 480/2-100;

    rcscoreMade.x = 640/2-120; /// Center of the screen
    rcscoreMade.y = 480/2-30;

    rcPlayer1_text.x = 640/2-100;
    rcPlayer1_text.y = 480-20;
    rcPlayer2_text.x = 640/2-100;
    rcPlayer2_text.y = 0;
    rcPlayer3_text.x = 0;
    rcPlayer3_text.y = 480/2-100;
    rcPlayer4_text.x = 640-15;
    rcPlayer4_text.y = 480/2-100;

    rcwall_p1.x = 0;
    rcwall_p1.y = 480-20;
    rcwall_p2.x = 0;
    rcwall_p2.y = 0;
    rcwall_p3.x = 0;
    rcwall_p3.y = 0;
    rcwall_p4.x = 640-20;
    rcwall_p4.y = 0;

    ///Music
    music = Mix_LoadMUS("beat.wav");
    effect = Mix_LoadWAV("low.wav");

	///Start position Player1
	rcPlayer1.x = 640/2-75;
	rcPlayer1.y = 480-50;
	///Start position Player2
	rcPlayer2.x = 640/2-75;
	rcPlayer2.y = 25;
	///Start position Player3
	rcPlayer3.x = 25;
	rcPlayer3.y = 480/2-75;
	///Start position Player4
	rcPlayer4.x = 640-50;
	rcPlayer4.y = 480/2-75;


	/*//Start ball position
    rcball.x = SCREEN_WIDTH/2-30;
    rcball.y = SCREEN_HEIGHT/2-30;*/

    ///Load StartMenu
    if(!loadMenu(gWindow, gScreenSurface, font, effect))
    {
        success = false;
    }

	return success;
}

void close()
{
	///Deallocate surface
	SDL_FreeSurface( gXOut );
	SDL_FreeSurface(Ball);
	SDL_FreeSurface(Player1);
	SDL_FreeSurface(Player2);
	SDL_FreeSurface(text1);
	SDL_FreeSurface(text2);
	gXOut = NULL;
	Ball = NULL;
	Player1 = NULL;
	Player2 = NULL;

	Mix_FreeChunk(effect);
	Mix_FreeMusic(music);
	Mix_CloseAudio();

	TTF_CloseFont(font);
	///Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

  TTF_Quit();
	///Quit SDL subsystems
	SDL_Quit();
}

void printScore() /// prints "score" on the screen
{
	int i;
	for(i=0;i<60;i++)/// Loop must be there otherwise it gets really f-uped and only shows "score" for 1ms
  {
    SDL_BlitSurface(scoreMade, NULL, gScreenSurface, &rcscoreMade);
    SDL_UpdateWindowSurface( gWindow );
    SDL_Delay( 20 );
  }
}
 
void checkExtraLifeWall(int points, bool &extraLife)
{
	if(points==1 && extraLife == false){
    SDL_BlitSurface(wall_p2,NULL,gScreenSurface,&rcwall_p2);
    extraLife = true;
  }
}

int main( int argc, char* args[] )
{
    /// Träffat kloss
    bool extraLife = false;
    int j=0, i=0;
    int life1 = 0, life2 = 0, life3 = 0, life4 = 0;
    srand(time(NULL));


	///Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		///Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{

      //Mix_PlayMusic(music,-1);

			///Main loop flag
			bool gameover = false;

			///Event handler
			SDL_Event event;

      ///Initialize Frames per second
      FPS_Init();

      RestartBall(rcball);     
      angle = rand() % 361;
      newDirectionBall(angle,rcball);

			///While application is running
			while( !gameover )
			{
				///Look for events
				if( SDL_PollEvent( &event ) )
				{
					///An event was found
					switch (event.type)
					{

                        ///Close button clicked
                        case SDL_QUIT:
                        gameover = 1;
                        break;

                        ///Handle the keybord
                        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                            case SDLK_q:
                                gameover = 1;
                                break;

                            case SDLK_LEFT:
                                rcPlayer1.x -= 20;
                                if(rcPlayer1.x < 0 )
                                {
                                    rcPlayer1.x = 0;
                                }
                                break;

                            case SDLK_RIGHT:
                                rcPlayer1.x += 20;
                                if(rcPlayer1.x > SCREEN_WIDTH - 150)
                                {
                                    rcPlayer1.x = SCREEN_WIDTH - 150;
                                }
                                break;

                            case SDLK_a:
                                rcPlayer2.x -= 20;
                                if(rcPlayer2.x < 0 )
                                {
                                    rcPlayer2.x = 0;
                                }
                                break;

                            case SDLK_d:
                                rcPlayer2.x += 20;
                                if(rcPlayer2.x > SCREEN_WIDTH - 150)
                                {
                                    rcPlayer2.x = SCREEN_WIDTH - 150;
                                }
                                break;

                            case SDLK_w:
                                rcPlayer3.y -= 20;
                                if(rcPlayer3.y < 0 )
                                {
                                    rcPlayer3.y = 0;
                                }
                                break;

                            case SDLK_s:
                                rcPlayer3.y += 20;
                                if(rcPlayer3.y > SCREEN_HEIGHT - 150)
                                {
                                    rcPlayer3.y = SCREEN_HEIGHT - 150;
                                }
                                break;

                            case SDLK_UP:
                                rcPlayer4.y -= 20;
                                if(rcPlayer4.y < 0 )
                                {
                                    rcPlayer4.y = 0;
                                }
                                break;

                            case SDLK_DOWN:
                                rcPlayer4.y += 20;
                                if(rcPlayer4.y > SCREEN_HEIGHT - 150)
                                {
                                    rcPlayer4.y = SCREEN_HEIGHT - 150;
                                }
                                break;

                        }
                        break;
					}
				} // if poll end



				///Apply the image
				SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );

        SDL_BlitSurface(Ball, NULL, gScreenSurface, &rcball);

        if(invisable && invisable_effekt == 1){
          SDL_BlitSurface(Player1, NULL, gScreenSurface, &rcPlayer1);
          SDL_BlitSurface(InvisablePlayer2, NULL, gScreenSurface, &rcPlayer2);
          SDL_BlitSurface(InvisablePlayer3, NULL, gScreenSurface, &rcPlayer3);
          SDL_BlitSurface(InvisablePlayer4, NULL, gScreenSurface, &rcPlayer4);
        }
        else if(invisable && invisable_effekt == 2){
          SDL_BlitSurface(InvisablePlayer1, NULL, gScreenSurface, &rcPlayer1);
          SDL_BlitSurface(Player2, NULL, gScreenSurface, &rcPlayer2);
          SDL_BlitSurface(InvisablePlayer3, NULL, gScreenSurface, &rcPlayer3);
          SDL_BlitSurface(InvisablePlayer4, NULL, gScreenSurface, &rcPlayer4);
        }
        else if(invisable && invisable_effekt == 3){
          SDL_BlitSurface(InvisablePlayer1, NULL, gScreenSurface, &rcPlayer1);
          SDL_BlitSurface(InvisablePlayer2, NULL, gScreenSurface, &rcPlayer2);
          SDL_BlitSurface(Player3, NULL, gScreenSurface, &rcPlayer3);
          SDL_BlitSurface(InvisablePlayer4, NULL, gScreenSurface, &rcPlayer4);
        }
        else if(invisable && invisable_effekt == 4){
          SDL_BlitSurface(InvisablePlayer1, NULL, gScreenSurface, &rcPlayer1);
          SDL_BlitSurface(InvisablePlayer2, NULL, gScreenSurface, &rcPlayer2);
          SDL_BlitSurface(InvisablePlayer3, NULL, gScreenSurface, &rcPlayer3);
          SDL_BlitSurface(Player4, NULL, gScreenSurface, &rcPlayer4);
        }
				else{
          SDL_BlitSurface(Player1, NULL, gScreenSurface, &rcPlayer1);
				  SDL_BlitSurface(Player2, NULL, gScreenSurface, &rcPlayer2);
				  SDL_BlitSurface(Player3, NULL, gScreenSurface, &rcPlayer3);
				  SDL_BlitSurface(Player4, NULL, gScreenSurface, &rcPlayer4);
        }

        if(points[1]==1 && life1==1){
            SDL_BlitSurface(wall_p1, NULL, gScreenSurface, &rcwall_p1);
        }
        if(points[2]==1 && life2==1){
            SDL_BlitSurface(wall_p2, NULL, gScreenSurface, &rcwall_p2);
        }
        if(points[3]==1 && life3==1){
            SDL_BlitSurface(wall_p3, NULL, gScreenSurface, &rcwall_p3);
        }
        if(points[4]==1 && life4==1){
            SDL_BlitSurface(wall_p4, NULL, gScreenSurface, &rcwall_p4);
        }

        SDL_BlitSurface(text1, NULL, gScreenSurface, &rctext1);
        SDL_BlitSurface(text2, NULL, gScreenSurface, &rctext2);
        SDL_BlitSurface(text3, NULL, gScreenSurface, &rctext3);
        SDL_BlitSurface(text4, NULL, gScreenSurface, &rctext4);

				SDL_UpdateWindowSurface( gWindow );

				/// PLAYER2s WALL
        if(rcball.y < 1){ ///Touched the top of the screen
          points[2]--;
          printScore();///prints "score" on the screen

          if(points[2]==1 && extraLife == false){/// check if players point is so low that he gets "the wall" of extra life
          	text2 = TTF_RenderText_Blended(font3,score[points[2]],colors[1]);
          	SDL_BlitSurface(wall_p2,NULL,gScreenSurface,&rcwall_p2);
          	extraLife = true;
          	life2++; /// this is for collitiondetection in the wall of extra life, look under each players collition detection

          }
          else{
          	text2 = TTF_RenderText_Blended(font3, score[points[2]], colors[points[2]]);
          }
          
          SDL_BlitSurface(text2, NULL, gScreenSurface, &rcPlayer2);
          
          RestartBall(rcball); /// resets ball position and speed   
          angle = rand() % 361;/// reset angel to a random one
          newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++;/// add 1 to points made in the game
          checkEffects(points_made,2);/// Checks if there is an effekt to happen att this many points made in the game
        }


        ///PLAYER1s WALL
        else if(rcball.y > (SCREEN_HEIGHT - rcball.h - 1) ){ /// touched the bottom of the screen
        	points[1]--;
          printScore();///prints "score" on the screen

          if(points[1]==1 && extraLife == false){
            text1 = TTF_RenderText_Blended(font3, score[points[1]], colors[points[1]]);
            SDL_BlitSurface(wall_p1, NULL, gScreenSurface, &rcwall_p1);
            extraLife = true;
            life1++;
          }
          else
          {
            text1 = TTF_RenderText_Blended(font3, score[points[1]], colors[points[1]]);
          }

          SDL_BlitSurface(text1, NULL, gScreenSurface, &rcPlayer1);/// prints out the new score of hearts

          RestartBall(rcball); /// resets ball position and speed  
          angle = rand() % 361; /// reset angel to a random one
          newDirectionBall(angle,rcball); /// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++; /// add 1 to points made in the game
          checkEffects(points_made,1); /// Checks if there is an effekt to happen att this many points made in the game
        }


        /// PLAYER3s WALL
        else if(rcball.x < 1){
          points[3]--;
          printScore();

          if(points[3]==1 && extraLife == false)
          {
            text3 = TTF_RenderText_Blended_Wrapped(font3, score[points[3]], colors[1],15);
            SDL_BlitSurface(wall_p3, NULL, gScreenSurface, &rcwall_p3);
            extraLife = true;
            rcball.y += 5;
            life3++;
          }
          else
          {
            text3 = TTF_RenderText_Blended_Wrapped(font3, score[points[3]], colors[points[3]],30);
          }
    
          SDL_BlitSurface(text3, NULL, gScreenSurface, &rcPlayer3);

					RestartBall(rcball);    /// resets ball position and speed  
          angle = rand() % 361;/// reset angel to a random one
          newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++;/// add 1 to points made in the game
          checkEffects(points_made,3);/// Checks if there is an effekt to happen att this many points made in the game
        }


        ///PLAYER4s WALL
        else if(rcball.x > (SCREEN_WIDTH - rcball.w -1)){
        	points[4]--;
        	printScore();

          if(points[4]==1 && extraLife == false)
          {
            text4 = TTF_RenderText_Blended_Wrapped(font3, score[points[4]], colors[1],15);
            SDL_BlitSurface(wall_p4, NULL, gScreenSurface, &rcwall_p4);
            extraLife = true;
            life4++;
          }
          else
          {
            text4 = TTF_RenderText_Blended_Wrapped(font3, score[points[4]], colors[points[4]],30);
          }
   
		      SDL_BlitSurface(text4, NULL, gScreenSurface, &rcPlayer4);

          RestartBall(rcball);  /// resets ball position and speed    
          angle = rand() % 361;/// reset angel to a random one
          newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++;/// add 1 to points made in the game
          checkEffects(points_made,4);/// Checks if there is an effekt to happen att this many points made in the game
        }


        /*========================COLLITION PLAYER1 =========================================*/
				if(Collition(rcPlayer1, rcball))/// Returns true if collition is detected
				{
          puts("True");
          if( (angle > 211 && angle < 329) || (angle > -149 && angle < -31) ){ /// To avoid changing of direction
            angle = 360 - angle - angleEffect(rcball,rcPlayer1,1);
          }
          else{
            angle = 360 - angle;
          }
          rcball.y -= 5;/// to avoid geting stuck and adds a "bounce"-effect 
          newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
				}

				if(life1==1)/// This is collition detection on the "wall of extra life", it will always bounce in an 90degree angle away from that wall
        {
					if(Collition(rcwall_p1, rcball))
					{ ///Collision with bonus wall
          	angle = 360 - angle;
          	puts("True");
          	life1--;
          	rcball.y -= 5;
          	newDirectionBall(angle,rcball);
					}
				}
				/*====================================================================================*/


				/*========================COLLITION PLAYER2 =========================================*/
				if(Collition(rcPlayer2, rcball))/// Returns true if collition is detected
				{
        	puts("True");
          if( (angle > 31 && angle < 149) || (angle > -329 && angle < -211) ){ /// To avoid non-acceptable direction
            angle = 360 - angle + angleEffect(rcball,rcPlayer2,2);
          }
          else{
            angle = 360 - angle;
          }
          rcball.y += 5;/// to avoid geting stuck and adds a "bounce"-effect 
    			newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
				}

				if(life2==1)/// This is collition detection on the "wall of extra life", it will always bounce in an 90degree angle away from that wall
        {
					if(Collition(rcwall_p2, rcball))
					{ ///Collision with bonus wall
          	angle = 360 - angle;
          	puts("True");
          	life2--;
          	rcball.y += 5;
          	newDirectionBall(angle,rcball);
   				}
   			}
   			/*====================================================================================*/


   			/*========================COLLITION PLAYER3 =========================================*/
				if(Collition(rcPlayer3, rcball))/// Returns true if collition is detected
				{
					puts("True");
          if( (angle > 121 && angle < 239) || (angle > -239 && angle < -121) ){
            angle = 180 - angle + angleEffect(rcball,rcPlayer3,3);
          }
          else{
            angle = 180 - angle;
          }
          rcball.x += 5;/// to avoid geting stuck and adds a "bounce"-effect 
    			newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
				}

				if(life3==1) /// This is collition detection on the "wall of extra life", it will always bounce in an 90degree angle away from that wall
        {
					if(Collition(rcwall_p3, rcball))
					{ ///Collision with bonus wall
          	angle = 180 - angle;
          	puts("True");
          	life3--;
        		rcball.x += 5;
          	newDirectionBall(angle,rcball);
        	}
        }
        /*====================================================================================*/


        /*========================COLLITION PLAYER4 =========================================*/
				if(Collition(rcPlayer4, rcball))/// Returns true if collition is detected
				{
        	puts("True");
          if( (angle > 301 || angle < 59) || (angle > -59 || angle < -301) )
          {
            angle = 180 - angle - angleEffect(rcball,rcPlayer4,4);
          }
          else
          {
            angle = 180 - angle;
          }
          rcball.x -= 5;/// to avoid geting stuck and adds a "bounce"-effect 
          newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
				}

				if(life4==1)/// This is collition detection on the "wall of extra life", it will always bounce in an 90degree angle away from that wall
        {
					if(Collition(rcwall_p4, rcball))
					{ ///Collision with bonus wall
          	angle = 180 - angle;
          	puts("True");
          	life4--;
          	rcball.x -= 5;
          	newDirectionBall(angle,rcball);
					}
				}

				/*====================================================================================*/

				MoveBall(rcball);

        FPS_Fn();
				///Update the surface
          
        /// If someone lost ( got 0 points) then we reload bunch of shit
        if(points[1]==0 || points[2]==0 || points[3]== 0 || points[4]==0)
        {///someone lost!
          for(i=0;i<=60;i++)
          {
            font4 = TTF_OpenFont("good.ttf", i);
            rcendofgame.x = gScreenSurface->clip_rect.w/2-rcendofgame.w/2;
            rcendofgame.y = 480/2-30;
            endofgame = TTF_RenderText_Blended(font4,"GAME OVER!",color[0]);
            SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
            SDL_BlitSurface(endofgame, NULL, gScreenSurface, &rcendofgame);
            SDL_UpdateWindowSurface( gWindow );
            SDL_Delay( 20 );
          }
          SDL_Delay( 3000 );


          /// The array that holds every players point is reset
          points[1]=9;
          points[2]=9;
          points[3]=9;
          points[4]=9;
          extraLife = false;

          if( !loadMedia() )
          {
            printf( "Failed to load media!\n" );
            return 0; ///Exit game
          }

          RestartBall(rcball); 
          angle = rand() % 361;
          newDirectionBall(angle,rcball);    
        }

			}/// End of while loop
		}
	}

	///Free resources and close SDL
	close();

	return 0;
}

