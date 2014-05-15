#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "window.h"

struct event
{
    int counter;
    char me[20];
    ENetPeer *peer;
};

ENetAddress address;
ENetHost *client;
ENetPeer *peer;
char message[1024];
char me[10];
ENetEvent netevent;
ENetPacket *packet;
int neteventStatus;
bool gameover = false;
bool FAILURE = false;
bool SUCCESS = true;
struct event input;
pthread_t Thread_id;


const int FRAME_PER_SECOND = 40;
int Intervall;
///Time controll
int NextTick;

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
    /* Allow up to 3 seconds for the disconnect to succeed
    * and drop any packets received packets.*/
    while (enet_host_service (client, & netevent, 3000) > 0)
    {
        switch (netevent.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy (netevent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts ("Disconnection succeeded.");
                close();
                return 0;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet. Force the connection down. */
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

    //Create a host using enet_host_create
    client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);

    if (client == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    //Connect to a server with IP address 130.237.84.99 on port 4950
    enet_address_set_host(&address, "130.237.84.99");
    address.port = 5950;

    //Connect and use service
    peer = enet_host_connect(client, &address, 2, 0);

    if (peer == NULL) {
        fprintf(stderr, "No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

    /* Try to connect to server within 5 seconds */
   if (enet_host_service (client, &netevent, 5000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_CONNECT)
   {
       //printf("Connection to server succeeded.\n\n");

   }

   else
   {
       /* Either the 5 seconds are up or a disconnect netevent was */
       /* received. Reset the peer in the netevent the 5 seconds   */
       /* had run out without any significant netevent.            */
       enet_peer_reset (peer);

       fprintf (stderr, "Connection to server failed\nIt could be full\n.");
       exit (EXIT_FAILURE);
   }

   if (enet_host_service (client, &netevent, 1000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_RECEIVE)
   {
        if(strcmp((char *)netevent.packet->data, "player1") == 0)
        {
            strcpy(me,(char *)netevent.packet->data);
            return SUCCESS;
        }
        else if(strcmp((char *)netevent.packet->data, "player2") == 0)
        {
            strcpy(me,(char *)netevent.packet->data);
            return SUCCESS;
        }

        else if(strcmp((char *)netevent.packet->data, "player3") == 0)
        {
            strcpy(me,(char *)netevent.packet->data);
            return SUCCESS;
        }

        else if(strcmp((char *)netevent.packet->data, "player4") == 0)
        {
            strcpy(me,(char *)netevent.packet->data);
            return SUCCESS;
        }

        else if(strcmp((char *)netevent.packet->data, "Server are full!") == 0)
        {
            printf("%s\n", (char*)netevent.packet->data);
            disconnect_from_server();
            return FAILURE;
        }

   }

}
void *deal_with_input(void* input)
{
    perror("Thread Created\n");
    /* Cast the cookie pointer to the right type. */
    struct event* p = (struct event*) input;
    char message_thread[1024];
    SDL_Event event;
    ENetPacket *packet_thread;
    fprintf(stderr, "%s", p->me);
    while(!gameover)
    {
        ///Look for events
            if( SDL_PollEvent( &event ) )
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
                            strcpy(message_thread, p->me);
                            strcat(message_thread, " left");
                            packet_thread = enet_packet_create(message_thread, sizeof(message_thread) +1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(peer, 0, packet_thread);
                            strcpy(message_thread, "");
                            break;

                        case SDLK_RIGHT:
                            strcpy(message_thread, p->me);
                            strcat(message_thread, " right");
                            packet_thread = enet_packet_create(message_thread, sizeof(message_thread) +1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(peer, 0, packet_thread);
                            strcpy(message_thread, "");
                            break;

                        case SDLK_UP:
                            strcpy(message_thread, p->me);
                            strcat(message_thread, " up");
                            packet_thread = enet_packet_create(message_thread, sizeof(message_thread) +1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(peer, 0, packet_thread);
                            strcpy(message_thread, "");
                            break;

                        case SDLK_DOWN:
                            strcpy(message_thread, p->me);
                            strcat(message_thread, " down");
                            packet_thread = enet_packet_create(message_thread, sizeof(message_thread) +1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(peer, 0, packet_thread);
                            strcpy(message_thread, "");
                            break;

                    }
                    break;
                }
            }
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
        //return;
   }

   else if(strstr(packet, "player2"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer2.x = atoi(tmp);
        printf("Player2: %d\n\n", rcPlayer2.x);
        //return;
   }

   else if(strstr(packet, "player3"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer1.x = atoi(tmp);
        printf("Player3: %d\n\n", rcPlayer3.y);
        //return;
   }

    else if(strstr(packet, "player4"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer1.x = atoi(tmp);
        printf("Player4: %d\n\n", rcPlayer4.y);
        //return;
   }

   else if(strstr(packet, "ball.x"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.x = atoi(tmp);
        printf("Ball.x: %d\n\n", rcball.x);
        //return;
   }

   else if(strstr(packet, "ball.y"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.y = atoi(tmp);
        printf("Ball.y: %d\n\n", rcball.y);
        //return;
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
            int update_game = 0;
			Update_The_Surface();
			bool Next = false;
			if (connect_to_server() == FAILURE)
			{
                return 1;
			}
			///thread arguments
			strcpy(input.me, me);
			input.peer = peer;
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


            void FPS_Init();
            while( !gameover )
            {
                while(update_game < 5)
                {
                    // If we had some netevent that interested us
                    if (enet_host_service(client, &netevent, 1000/40))
                    {
                        switch(netevent.type)
                        {
                            case ENET_EVENT_TYPE_RECEIVE:
                            printf("(Client) Message from server : %s\n", netevent.packet->data);
                            decode_packet((char *)netevent.packet->data);
                            enet_packet_destroy(netevent.packet);
                            update_game++;
                            break;

                            case ENET_EVENT_TYPE_DISCONNECT:
                            printf("(Client) %s disconnected.\n", (char* )netevent.peer->data);

                            // Reset client's information
                            netevent.peer->data = NULL;
                            break;

                            case ENET_EVENT_TYPE_NONE:
                            update_game++;
                        }
                    }
                }
            //NextTick = SDL_GetTicks() + Intervall;
            update_game = 0;
            ///Apply the image
           // void FPS_Fn();
            printf("UPDATE SURFACE\n");
            Update_The_Surface();
            }
        }
    }
    pthread_join(Thread_id, NULL);
    disconnect_from_server();
    return 0;
}
