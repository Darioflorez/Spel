#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "window.h"


struct event
{
    int counter;
    char me[20];
};

ENetAddress address;
ENetHost *client;
ENetPeer *peer;
char message[1024];
char me[20];
ENetEvent netevent;
ENetPacket *packet;
int neteventStatus;
SDL_Event event;
bool gameover = false;
bool FAILURE = false;
bool SUCCESS = true;

///Thread
struct event input;
pthread_t Thread_id;
const int FRAME_PER_SECOND = 50;
int Intervall;
///Time controll
int NextTick;

void Send_Players_and_Ball_Info()
{

    sprintf(message,"%d %d %d %d", rcPlayer1.w, rcPlayer1.h, rcball.w, rcball.h);
    packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

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


int disconnect_from_server()
{
    printf("Disconnecting....\n");
    enet_peer_disconnect (peer, 0);
   /// Allow up to 3 seconds for the disconnect to succeed
    /// and drop any packets received packets.
    while (enet_host_service (client, & netevent, 3000) > 0)
    {
        switch (netevent.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy (netevent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts ("Disconnection succeeded.....");
                ///Free resources and close SDL
                close();
                return 0;
        }
    }
    ///We've arrived here, so the disconnect attempt didn't
    ///succeed yet. Force the connection down.
    enet_peer_reset (peer);
    ///Free resources and close SDL
	close();
	enet_deinitialize();
    return 0;

}

bool connect_to_server()
{
        //Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    ///Create a host using enet_host_create
    client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);

    if (client == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    ///Connect to a server with IP address 130.237.84.99 on port 4950//My ip 80.217.155.111
    enet_address_set_host(&address, "130.237.84.99");
    address.port = 5950;

    ///Connect and use service
    peer = enet_host_connect(client, &address, 2, 0);

    if (peer == NULL) {
        fprintf(stderr, "No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

    /// Try to connect to server within 5 seconds
   if (enet_host_service (client, &netevent, 5000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_CONNECT)
   {
       printf("Connection to server succeeded.\n\n");

   }

   else
   {
        ///Either the 5 seconds are up or a disconnect netevent was
        ///received. Reset the peer in the netevent the 5 seconds
        ///had run out without any significant netevent.
       enet_peer_reset (peer);

       fprintf (stderr, "Connection to server failed\nServer could be full\n.");
       exit (EXIT_FAILURE);
   }

   if (enet_host_service (client, &netevent, 1000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_RECEIVE)
   {
        if(strcmp((char *)netevent.packet->data, "Server are full!") == 0)
        {
            printf("%s\n", (char*)netevent.packet->data);
            disconnect_from_server();
            return FAILURE;
        }

        else if(strcmp((char *)netevent.packet->data, "player4") == 0)
        {
            strcpy(me,(char *)netevent.packet->data);
            //Skicka data om players and ball
            Send_Players_and_Ball_Info();
            return SUCCESS;
        }

        else 
        {
            printf("%s\n", (char*)netevent.packet->data);
            return SUCCESS;
        }

   }

}



void *deal_with_input(void* input)
{
    perror("Thread Created\n");
    /* Cast the cookie pointer to the right type. */
    struct event* p = (struct event*) input;
    fprintf(stderr, "%s", p->me);
    //FPS_Init();

    while(!gameover)
    {

        ///Look for events
        while( SDL_PollEvent( &event ) )
        {
        ///An event was found
        switch (event.type)
        {
            ///Close button clicked
            case SDL_QUIT:
                gameover = true;
                break;

            ///Handle the keybord
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        gameover = true;
                        break;

                    case SDLK_LEFT:
                        strcpy(message, p->me);
                        strcat(message, " left");
                        packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(peer, 0, packet);
                        strcpy(message, "");
                        break;

                    case SDLK_RIGHT:
                        strcpy(message, p->me);
                        strcat(message, " right");
                        packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(peer, 0, packet);
                        strcpy(message, "");
                        break;

                    case SDLK_UP:
                        strcpy(message, p->me);
                        strcat(message, " up");
                        packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(peer, 0, packet);
                        strcpy(message, "");
                        break;

                    case SDLK_DOWN:
                        strcpy(message, p->me);
                        strcat(message, " down");
                        packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(peer, 0, packet);
                        strcpy(message, "");
                        break;

                }
                break;
            }
        }
        ///Frames per second
        //FPS_Fn();
        ///Apply the image
        //Update_The_Surface();
    }
}

void decode_packet(char* packet)
{
    char tmp[20];
    int i;
   if(strstr(packet, "player1"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer1.x = atoi(tmp);
        printf("Player1: %d\n\n", rcPlayer1.x);
   }

   else if(strstr(packet, "player2"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer2.x = atoi(tmp);
        printf("Player2: %d\n\n", rcPlayer2.x);
   }

   else if(strstr(packet, "player3"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer3.y = atoi(tmp);
        printf("Player3: %d\n\n", rcPlayer3.y);
   }

    else if(strstr(packet, "player4"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer4.y = atoi(tmp);
        printf("Player4: %d\n\n", rcPlayer4.y);
   }

   else if(strstr(packet, "ball.x"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.x = atoi(tmp);
        printf("Ball.x: %d\n\n", rcball.x);
   }

   else if(strstr(packet, "ball.y"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.y = atoi(tmp);
        printf("Ball.y: %d\n\n", rcball.y);
   }
}

int main(int argc, char **argv)
{
    pthread_t Thread_id;
    ///Start up SDL and create window
	if( !create_window())
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
            ///Event handler
            ///#################Här måste menyn slängas in######################
			Update_The_Surface();
			if (connect_to_server() == FAILURE)
			{
                return EXIT_FAILURE;
			}


			///thread arguments and create thread
			strcpy(input.me, me);
			pthread_create(&Thread_id, NULL, &deal_with_input, &input);

			/*while( !Next )
			{
                if( SDL_PollEvent( &event ) )
                {
                    ///An event was found
                    switch (event.type)
                    {
                        ///Close button clicked
                        case SDL_QUIT:
                        gameover = true;
                        Next = true;
                        break;
                        ///Handle the keybord
                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym)
                            {
                                case SDLK_ESCAPE:
                                case SDLK_q:
                                gameover = true;
                                Next = true;
                                break;

                                case SDLK_TAB:
                                Next = true;
                                break;
                            }
                        break;
                    }
                }
            }*/
            FPS_Init();
            while(!gameover)
            {
                    // If we had some netevent that interested us
                while(enet_host_service(client, &netevent, 0))
                {
                    switch(netevent.type)
                    {
                        case ENET_EVENT_TYPE_RECEIVE:
                            printf("(Client) Message from server : %s\n", netevent.packet->data);
                            decode_packet((char *)netevent.packet->data);
                            enet_packet_destroy(netevent.packet);
                            break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                            printf("(Client) %s Server disconnected.....\n\nGAME OVER!!!", (char* )netevent.peer->data);
                            gameover = true;
                            /// Reset client's information
                            netevent.peer->data = NULL;
                            break;

                        case ENET_EVENT_TYPE_NONE:
                            break;
                    }
                }
                ///Frames per second
        FPS_Fn();
        ///Apply the image
        Update_The_Surface();
            }

        }

    }
    pthread_join(Thread_id, NULL);
    disconnect_from_server();
    return EXIT_SUCCESS;
}
