#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <enet/enet.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <SDL2/SDL.h>


#define MAX 100
#define LEFT 1
#define RIGHT 2
#define UP  3
#define DOWN 4


struct event
{
    int counter;
};

/*typedef struct
{
    int x;
    int y;
    double acc_vel;
    double angle;
    double scale_x, scale_y;
    double Vel_x, Vel_y;
    ///Make the ball move smoothly
    int StartPosition_x, StartPosition_y;
}Info_Ball;*/

///Ball Parameter
SDL_Rect rcball;
double acc_vel = 5;
double angle = 0, angle2=0;
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
int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 700;
int player1_input;
int player2_input;
int player3_input;
int player4_input;
ENetPacket *Thread_packet;
char Thread_message[MAX];
ENetHost *server;


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
const int FRAME_PER_SECOND = 30;
int Intervall;
///Time controll
int NextTick;

///Players
SDL_Rect rcPlayer1;
SDL_Rect rcPlayer2;
SDL_Rect rcPlayer3;
SDL_Rect rcPlayer4;
//Info_Ball Ball1;


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
  ball.x = SCREEN_WIDTH/2-30;
  ball.y = SCREEN_HEIGHT/2-30;
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

void *ball_move(void *input)
{
  /* Cast the cookie pointer to the right type. */
    struct event* p = (struct event*) input;
    p->counter=0;
    FPS_Init();

    while(!gameover)
    {
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
            rcPlayer1.x -= 10;
            if(rcPlayer1.x < 0 )
            {
                rcPlayer1.x = 0;
            }
            sprintf(message, "player1 %d", rcPlayer1.x);
            Broadcast_Packet(message, packet);
            player1_input=0;
            break;

        case RIGHT:
            rcPlayer1.x += 10;
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
            rcPlayer2.x -= 10;
            if(rcPlayer2.x < 0 )
            {
                rcPlayer2.x = 0;
            }
            sprintf(message, "player2 %d", rcPlayer2.x);
            Broadcast_Packet(message, packet);
            player2_input = 0;
            break;

            case RIGHT:
                rcPlayer2.x += 10;
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
            rcPlayer3.y -= 10;
            if(rcPlayer3.y < 0 )
            {
                rcPlayer3.y = 0;
            }
            sprintf(message, "player3 %d", rcPlayer3.y);
            Broadcast_Packet(message, packet);
            player3_input = 0;
            break;

        case DOWN:
            rcPlayer3.y += 10;
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
            rcPlayer4.y -= 10;
            if(rcPlayer4.y < 0 )
            {
                rcPlayer4.y = 0;
            }
            sprintf(message, "player4 %d", rcPlayer4.y);
            Broadcast_Packet(message, packet);
            player4_input = 0;
            break;

        case DOWN:
            rcPlayer4.y += 10;
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
    resetPlayerPosition();
	RestartBall(rcball);
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
