#include "window.h"


///The window we'll be rendering to
SDL_Window *Window = NULL;
///The surface contained by the window
SDL_Surface *ScreenSurface = NULL;

///The image we will load and show on the screen
SDL_Surface* XOut = NULL;

//When you score a point
SDL_Surface* scoreMade = NULL;
SDL_Rect rcscoreMade;

///The ball
SDL_Surface* Ball = NULL;
///Ball position
SDL_Rect rcball;

///ALL Players
SDL_Surface* Player1 = NULL;
SDL_Surface* Player2 = NULL;
SDL_Surface* Player3 = NULL;
SDL_Surface* Player4 = NULL;

SDL_Surface* Player1_text = NULL;
SDL_Surface* Player2_text = NULL;
SDL_Surface* Player3_text = NULL;
SDL_Surface* Player4_text =NULL;

SDL_Rect rcPlayer1;
SDL_Rect rcPlayer2;
SDL_Rect rcPlayer3;
SDL_Rect rcPlayer4;

SDL_Rect rcPlayer1_text;
SDL_Rect rcPlayer2_text;
SDL_Rect rcPlayer3_text;
SDL_Rect rcPlayer4_text;


//Invisible players
SDL_Surface* InvisablePlayer1;
SDL_Surface* InvisablePlayer2;
SDL_Surface* InvisablePlayer3;
SDL_Surface* InvisablePlayer4;


//Wall
SDL_Surface* wall_p1 = NULL;
SDL_Rect rcwall_p1;

SDL_Surface* wall_p2 = NULL;
SDL_Rect rcwall_p2;

SDL_Surface* wall_p3 = NULL;
SDL_Rect rcwall_p3;

SDL_Surface* wall_p4 = NULL;
SDL_Rect rcwall_p4;


//Score
SDL_Surface* text1;
SDL_Surface* text2;
SDL_Surface* text3;
SDL_Surface* text4;


SDL_Color colors[] = {{255,0,0},{255,51,0},{255,102,0},{255,153,0},{255,204,0},{255,255,0},{204,255,0},{153,255,0},{102,255,0},{51,255,0},{0,255,0}};
//{{0,255,0},{51,255,0},{102,255,0},{153,255,0},{204,255,0},{255,255,0},{255,204,0},{255,153,0},{255,102,0},{255,51,0},{255,0,0}};


SDL_Rect rctext1;
SDL_Rect rctext2;
SDL_Rect rctext3;
SDL_Rect rctext4;

SDL_Color color[2] = {{255,255,255},{255,0,0}};

///Font
TTF_Font* font = NULL;
TTF_Font* font2 = NULL;
TTF_Font* font3 = NULL;
TTF_Font* font4 = NULL;

const char* score[] = {" ","D","D D","D D D","D D D D","D D D D D","D D D D D D","D D D D D D D","D D D D D D D D","D D D D D D D D D","D D D D D D D D D D"};
//{"DDDDDDDDDD","DDDDDDDDD","DDDDDDDD","DDDDDDD","DDDDDD","DDDDD","DDDD","DDD","DD","D","Game Over"}; ///Hearts

int points[5] = {0,9,9,9,9};


bool create_window()
{
    bool success = true;
    ///Initialized SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        ///Create window           ///Title
        Window = SDL_CreateWindow("Pingpong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Window == 0)
        {
            printf("Window could not be created, SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            ///Get a window surface
            ScreenSurface = SDL_GetWindowSurface(Window);
        }

    }

    ///Initialize TTF
	if (TTF_Init() == -1)
	{
        return false;
	}
    return success;
}

bool loadMedia()
{

	///Loading success flag
	bool success = true;

	///Load splash image
	XOut = SDL_LoadBMP( "black.bmp" );
	if( XOut == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "black.bmp", SDL_GetError() );
		success = false;
	}

	///Load a ball
	Ball = SDL_LoadBMP("ball4.bmp");
	if(Ball == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "ball.bmp", SDL_GetError() );
		success = false;
	}

	///Load all Players
	Player1 = SDL_LoadBMP("block_p1_p2.bmp");
	if(Player1 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "block_p1_p2.bmp", SDL_GetError() );
		success = false;
	}
	Player2 = SDL_LoadBMP("block_p1_p2.bmp");
	if(Player2 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "block_p1_p2.bmp", SDL_GetError() );
		success = false;
	}
	Player3 = SDL_LoadBMP("block_p3_p4.bmp");
	if(Player3 == NULL)
	{
        printf( "Unable to load image %s! SDL Error: %s\n", "rectangle_blue.bmp", SDL_GetError() );
		success = false;
	}
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
    if(font4 == NULL)
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

    //Blitsurface to save the data of the wall players and ball to send to the server
    SDL_BlitSurface(wall_p1, NULL, ScreenSurface, &rcwall_p1);
    SDL_BlitSurface(wall_p2, NULL, ScreenSurface, &rcwall_p2);
    SDL_BlitSurface(wall_p3, NULL, ScreenSurface, &rcwall_p3);
    SDL_BlitSurface(wall_p4, NULL, ScreenSurface, &rcwall_p4);
    SDL_BlitSurface(Player1, NULL, ScreenSurface, &rcPlayer1);
    SDL_BlitSurface(Player2, NULL, ScreenSurface, &rcPlayer2);
    SDL_BlitSurface(Player3, NULL, ScreenSurface, &rcPlayer3);
    SDL_BlitSurface(Player4, NULL, ScreenSurface, &rcPlayer4);
    SDL_BlitSurface(Ball, NULL, ScreenSurface, &rcball);

	///Start position Player1
	rcPlayer1.x = SCREEN_WIDTH/2-75;
	rcPlayer1.y = SCREEN_HEIGHT-50;
	///Start position Player2
	rcPlayer2.x = SCREEN_WIDTH/2-75;
	rcPlayer2.y = 25;
	///Start position Player3
	rcPlayer3.x = 25;
	rcPlayer3.y = SCREEN_HEIGHT/2-75;
	///Start position Player4
	rcPlayer4.x = SCREEN_WIDTH-50;
	rcPlayer4.y = SCREEN_HEIGHT/2-75;

	//Players text
	rcPlayer1_text.x = SCREEN_WIDTH/2-100;
    rcPlayer1_text.y = SCREEN_HEIGHT-20;
    rcPlayer2_text.x = SCREEN_WIDTH/2-100;
    rcPlayer2_text.y = 0;
    rcPlayer3_text.x = 0;
    rcPlayer3_text.y = SCREEN_HEIGHT/2-100;
    rcPlayer4_text.x = SCREEN_WIDTH-15;
    rcPlayer4_text.y = SCREEN_HEIGHT/2-100;

    //Balls start position
    rcball.x = SCREEN_WIDTH/2-rcball.w;
    rcball.y = SCREEN_HEIGHT/2-rcball.h;

    //Hearts
    rctext1.x = SCREEN_WIDTH/2-100;
    rctext1.y = SCREEN_HEIGHT-20;
    rctext2.x = SCREEN_WIDTH/2-100;
    rctext2.y = 0;
    rctext3.x = 0;
    rctext3.y = SCREEN_HEIGHT/2-100;
    rctext4.x = SCREEN_WIDTH-20;
    rctext4.y = SCREEN_HEIGHT/2-100;

    rcscoreMade.x = SCREEN_WIDTH/2-120; /// Center of the screen
    rcscoreMade.y = SCREEN_HEIGHT/2-30;

    rcwall_p1.x = 0;
    rcwall_p1.y = SCREEN_HEIGHT-20;
    rcwall_p2.x = 0;
    rcwall_p2.y = 0;
    rcwall_p3.x = 0;
    rcwall_p3.y = 0;
    rcwall_p4.x = SCREEN_WIDTH-20;
    rcwall_p4.y = 0;

	return success;
}

void close()
{
	///Deallocate surface
	SDL_FreeSurface( XOut );
	SDL_FreeSurface(Ball);
	SDL_FreeSurface(Player1);
	SDL_FreeSurface(Player2);
	SDL_FreeSurface(Player3);
	SDL_FreeSurface(Player4);
	XOut = NULL;
	Ball = NULL;
	Player1 = NULL;
	Player2 = NULL;
	Player3 = NULL;
	Player4 = NULL;

	///Destroy window
	SDL_DestroyWindow( Window );
	Window = NULL;

	//Mix_FreeChunk(effect);
	//Mix_FreeMusic(music);
	//Mix_CloseAudio();
	TTF_CloseFont(font);
	TTF_Quit();
	///Quit SDL subsystems
	SDL_Quit();
}
