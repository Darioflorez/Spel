#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <enet/enet.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <SDL2/SDL.h>


#define MAX 100
#define LEFT 1
#define RIGHT 2
#define UP  3
#define DOWN 4
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT  480


struct event
{
    int counter;
};

double acc_vel = 5;
double angle = 0;
double scale_x= 0, scale_y=0;
double Resultante=0;
double Vel_x=0, Vel_y=0;
///Make the ball move smoothly
int StartPosition_x=0, StartPosition_y=0;

///functions
void *ball_move(void *input);
void decode_packet(char *packet);
void Player_Action();
void MoveBall();
void RestartBall();
void Broadcast_Packet(char *message, ENetPacket *packet);
void FPS_Fn();
void FPS_Init();

///Variables
struct event input;
pthread_t Thread_id;
bool gameover = false;
const double    PI = 3.14159265;
int acc_distance;
int player1_input;
int player2_input;
int player3_input;
int player4_input;
ENetPacket *Thread_packet;
char Thread_message[MAX];
ENetHost *server;

///NY VAR
int points[5]={0,9,9,9,9};
int points_made = 0;



///Det var i main
ENetAddress address;
ENetPeer *client[5];
ENetEvent event;
ENetEvent event_game;
ENetPacket *packet;
int eventStatus;
int New_Client = 0;
char return_message[50];
char message[50];


///Timer
const int FRAME_PER_SECOND = 60;
int Intervall;
///Time controll
int NextTick;

///Players
SDL_Rect rcPlayer1;
SDL_Rect rcPlayer2;
SDL_Rect rcPlayer3;
SDL_Rect rcPlayer4;
///Ball Parameter
SDL_Rect rcball;


void FPS_Fn()
{
    if(NextTick > SDL_GetTicks())
    {
        SDL_Delay(NextTick - SDL_GetTicks());
    }
    NextTick = SDL_GetTicks() + Intervall;
}

void FPS_Init()
{
    NextTick = 0;
    Intervall = 1*1000/FRAME_PER_SECOND;
    return;
}

void Broadcast_Packet(char *message, ENetPacket *packet)
{
    packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast (server, 0, packet);

}

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
  ball.x = SCREEN_WIDTH/2-ball.w;
  ball.y = SCREEN_HEIGHT/2-ball.h;
}


void MoveBall(struct SDL_Rect &ball)
{
    ball.x = StartPosition_x + (Vel_x + (Vel_x * acc_distance));
    ball.y = StartPosition_y + (Vel_y + (Vel_y * acc_distance));
    acc_distance++;
}


void decode2(char *packet)
{
    char tmp[20];
    if(strstr(packet, "player1 left"))
    {
        player1_input = LEFT;
    }
    else if(strstr(packet, "player1 right"))
    {
        player1_input = RIGHT;
    }

    else if(strstr(packet, "player2 left"))
    {
        player2_input = LEFT;
    }
    else if(strstr(packet, "player2 right"))
    {
        player2_input = RIGHT;
    }

    else if(strstr(packet, "player3 up"))
    {
        player3_input = UP;
    }
    else if(strstr(packet, "player3 down"))
    {
        player3_input = DOWN;
    }

    else if(strstr(packet, "player4 up"))
    {
        player4_input = UP;
    }
    else if(strstr(packet, "player4 down"))
    {
        player4_input = DOWN;
    }
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
	int ball_r = 25/2;
	double dis, dis_y, dis_x;

	fprintf(stderr,"%d\n\n%d\n", ball.w, player.h );

    perror(">VI ÄR INNE!!!!!\n\n");

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
		perror("BALL:");
        fprintf(stderr, "%d\n", ball_r);
        perror("DISTANCE:");
        fprintf(stderr,"%f\n", distance(ball_x,ball_y,px,py) );

		if( distance(ball_x,ball_y,px,py) < ball_r){
			perror("COLLISION\n\n");
			return true;
		}
	}
	perror("ICKE COLLISION\n\n");
	return false;
}


void *ball_move(void *input)
{
  /* Cast the cookie pointer to the right type. */
    struct event* p = (struct event*) input;
    p->counter=0;
    FPS_Init();

    while(!gameover)
    {
        ///PLAYER2s WALL
        if(rcball.y < 1){ ///Touched the top of the screen
            points[2]--;
            ///SKICKA POINTS
          RestartBall(rcball); /// resets ball position and speed
          angle = rand() % 361;/// reset angel to a random one
          newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++;/// add 1 to points made in the game
        }


        ///PLAYER1s WALL
        else if(rcball.y > (SCREEN_HEIGHT - rcball.h - 1) ){ /// touched the bottom of the screen
            points[1]--;
          ///SKICKA PRINTSCORE
          RestartBall(rcball); /// resets ball position and speed
          angle = rand() % 361; /// reset angel to a random one
          newDirectionBall(angle,rcball); /// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++; /// add 1 to points made in the game
        }

        /// PLAYER3s WALL
        else if(rcball.x < 1){
            points[3]--;
          ///SKICKA PRINTSCORE
            RestartBall(rcball);    /// resets ball position and speed
            angle = rand() % 361;/// reset angel to a random one
            newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
            points_made ++;/// add 1 to points made in the game
        }

        ///PLAYER4s WALL
        else if(rcball.x > (SCREEN_WIDTH - rcball.w -1)){
        	points[4]--;
        	///SKICKA PRINTSCORE

          RestartBall(rcball);  /// resets ball position and speed
          angle = rand() % 361;/// reset angel to a random one
          newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
          points_made ++;/// add 1 to points made in the game
        }
        //PLAYERS POINTS
        sprintf(message, "points %d %d %d %d", points[1], points[2], points[3], points[4]);
        Broadcast_Packet(message, packet);

        if (Collition(rcPlayer1, rcball))/// Returns true if collition is detected //if(rcball.y > rcPlayer1.y)///
        {
            perror("True");
            if( (angle > 211 && angle < 329) || (angle > -149 && angle < -31) ){ /// To avoid changing of direction
                angle = 360 - angle - angleEffect(rcball,rcPlayer1,1);
            }
            else{
                angle = 360 - angle;
            }
            rcball.y -= 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
        }

        if (Collition(rcPlayer2, rcball))/// Returns true if collition is detected ///if(rcball.y < rcPlayer2.y)///
        {
            perror("True");
            if( (angle > 31 && angle < 149) || (angle > -329 && angle < -211) ){ /// To avoid non-acceptable direction
                angle = 360 - angle + angleEffect(rcball,rcPlayer2,2);
            }
            else{
                angle = 360 - angle;
            }
            rcball.y += 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
        }


		if(Collition(rcPlayer3, rcball))/// Returns true if collition is detected
        {
            perror("True");
            if( (angle > 121 && angle < 239) || (angle > -239 && angle < -121) ){
                angle = 180 - angle + angleEffect(rcball,rcPlayer3,3);
            }
            else{
                angle = 180 - angle;
            }
            rcball.x += 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location
        }


        if(Collition(rcPlayer4, rcball))/// Returns true if collition is detected
        {
        	perror("True");
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

        MoveBall(rcball);
        sprintf(message, "ball.x %d", rcball.x);
        Broadcast_Packet(message, packet);
        sprintf(message, "ball.y %d", rcball.y);
        Broadcast_Packet(message, packet);
        FPS_Fn();
    }

}

void Player_Action()
{
    switch(player1_input)
    {
        case LEFT:
            rcPlayer1.x -= 20;
            if(rcPlayer1.x < 0 )
            {
                rcPlayer1.x = 0;
            }
            sprintf(message, "player1 %d", rcPlayer1.x);
            Broadcast_Packet(message, packet);
            player1_input=0;
            break;

        case RIGHT:
            rcPlayer1.x += 20;
            if(rcPlayer1.x > SCREEN_WIDTH - 150)
            {
                rcPlayer1.x = SCREEN_WIDTH - 150;
            }
            sprintf(message, "player1 %d", rcPlayer1.x);
            Broadcast_Packet(message, packet);
            player1_input=0;
            break;
    }

    switch(player2_input)
    {
        case LEFT:
            rcPlayer2.x -= 20;
            if(rcPlayer2.x < 0 )
            {
                rcPlayer2.x = 0;
            }
            sprintf(message, "player2 %d", rcPlayer2.x);
            Broadcast_Packet(message, packet);
            player2_input = 0;
            break;

            case RIGHT:
                rcPlayer2.x += 20;
                if(rcPlayer2.x > SCREEN_WIDTH - 150)
                {
                    rcPlayer2.x = SCREEN_WIDTH - 150;
                }
                sprintf(message, "player2 %d", rcPlayer2.x);
                Broadcast_Packet(message, packet);
                player2_input = 0;
                break;
    }

    switch(player3_input)
    {
        case UP:
            rcPlayer3.y -= 20;
            if(rcPlayer3.y < 0 )
            {
                rcPlayer3.y = 0;
            }
            sprintf(message, "player3 %d", rcPlayer3.y);
            Broadcast_Packet(message, packet);
            player3_input = 0;
            break;

        case DOWN:
            rcPlayer3.y += 20;
            if(rcPlayer3.y > SCREEN_HEIGHT - 150)
            {
                rcPlayer3.y = SCREEN_HEIGHT - 150;
            }
            sprintf(message, "player3 %d", rcPlayer3.y);
            Broadcast_Packet(message, packet);
            player3_input = 0;
            break;
    }

    switch(player4_input)
    {
        case UP:
            rcPlayer4.y -= 20;
            if(rcPlayer4.y < 0 )
            {
                rcPlayer4.y = 0;
            }
            sprintf(message, "player4 %d", rcPlayer4.y);
            Broadcast_Packet(message, packet);
            player4_input = 0;
            break;

        case DOWN:
            rcPlayer4.y += 20;
            if(rcPlayer4.y > SCREEN_HEIGHT - 150)
            {
                rcPlayer4.y = SCREEN_HEIGHT - 150;
            }
            sprintf(message, "player4 %d", rcPlayer4.y);
            Broadcast_Packet(message, packet);
            player4_input = 0;
            break;
    }

}

void resetPlayerPosition(){
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
}




int main(int argc, char **argv)
{
    srand(time(NULL));
	///Init_Game;;;; Gör en sådant funktion!!!!!!!!!!

    if (enet_initialize()!=0)
    {
        fprintf(stderr,"An error ocurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    /*Bind the server to the default host. */
    /*A specific host address can be specified by
    enet_address_set_host(& address, "x.x.x.x");*/

    address.host = ENET_HOST_ANY;
    /*Bind the server to port 5950*/
    address.port = 5950;

    server = enet_host_create(& address, ///The address to bind the server host to
                              4,         ///Allow up to 2 clients and/or outgoing connetions
                              2,         ///Allow up to 2 channels to be used, 0 and 1
                              0,         ///Assume any amunt of incoming bandwidth
                              0          ///Assume any amount of utgoing bandwidth
                                );
    if (server == NULL)
    {
       fprintf(stderr, "An error occurred while trying to create an ENet server host. \n");
       exit(EXIT_FAILURE);
    }

    ///Connection loop
    while(New_Client<4)
    {
        /* if we had some event that interested us*/
        enet_host_service(server, &event, 1000);
        printf("Waiting for clients......\n");
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                    New_Client++;
                    printf ("A new client connected from Address: %x:%d\n",
                    event.peer -> address.host,event.peer->address.port);
                    ///Save all the new clients i en array
                    client[New_Client] = event.peer;

                    ///Make a packet to comunnicate to the client who player it gets
                    sprintf(return_message, "player%d", New_Client);
                    packet = enet_packet_create(return_message, strlen(return_message) + 1, ENET_PACKET_FLAG_RELIABLE);
                    enet_peer_send(event.peer, 0, packet);
                    printf("Peer: %x\n", client[New_Client]->address.host);
                    printf("%d\n", New_Client);
                    break;

        }
    }

    ///Nu har vi 4 clienter ta emot info om players and ball
    if (enet_host_service (server, &event, 2000) > 0 &&
       event.type == ENET_EVENT_TYPE_RECEIVE)
    {

        int i;
        sscanf ((char*)event.packet->data, "%d %d %d %d %d %d", 
            &rcPlayer1.w, &rcPlayer1.h, &rcball.w, &rcball.h, &rcPlayer3.w, &rcPlayer3.h);

        rcPlayer2.w = rcPlayer1.w;
        rcPlayer2.h = rcPlayer1.h;

        rcPlayer4.w = rcPlayer3.w;
        rcPlayer4.h = rcPlayer3.h;

    }
    
    //Start spelet
    resetPlayerPosition();
    RestartBall(rcball);
    angle = rand() % 361;
    newDirectionBall(angle,rcball);


    ///Create a Thread to make all operations and send data to clients
    pthread_create(&Thread_id, NULL, &ball_move, &input);

    while(!gameover)
    {
        while(enet_host_service(server, &event, 0))
        {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    ///DECODE
                    decode2((char*)event.packet->data);
                    ///Broadcast Packet
                    Player_Action();
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Client disconected....\n\nGAME OVER!!!\n\n");
                    gameover = true;
                    break;

            }
        }
    }
    pthread_join(Thread_id, NULL);
    enet_host_destroy(server);
    enet_deinitialize();
    return EXIT_SUCCESS;
}
