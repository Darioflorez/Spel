#include "window.h"

///Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

///The window we'll be rendering to
SDL_Window *Window = NULL;
///The surface contained by the window
SDL_Surface *ScreenSurface = NULL;

///The image we will load and show on the screen
SDL_Surface* XOut = NULL;

///The ball
SDL_Surface* Ball = NULL;
///Ball position
SDL_Rect rcball;

///ALL Players
SDL_Surface* Player1;
SDL_Surface* Player2;
SDL_Surface* Player3;
SDL_Surface* Player4;

SDL_Rect rcPlayer1;
SDL_Rect rcPlayer2;
SDL_Rect rcPlayer3;
SDL_Rect rcPlayer4;




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



	///Start position Player1
	rcPlayer1.x = 640/2-75;
	rcPlayer1.y = 480-30;

	///Start position Player2
	rcPlayer2.x = 640/2-75;
	rcPlayer2.y = 5;

	///Start position Player3
    rcPlayer3.x =25;
    rcPlayer3.y =480/2-75;

	///Start position Player4
    rcPlayer4.x = 640-50;
    rcPlayer4.y = 480/2-75;
	///Ball start position
	rcball.x = SCREEN_WIDTH/2-30;
    rcball.y = SCREEN_HEIGHT/2-30;

	return success;
}

void close()
{
	///Deallocate surface
	SDL_FreeSurface( XOut );
	SDL_FreeSurface(Ball);
	SDL_FreeSurface(Player1);
	SDL_FreeSurface(Player2);
	XOut = NULL;
	Ball = NULL;
	Player1 = NULL;
	Player2 = NULL;

	///Destroy window
	SDL_DestroyWindow( Window );
	Window = NULL;

	///Quit SDL subsystems
	SDL_Quit();
}
