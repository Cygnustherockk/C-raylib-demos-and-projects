#define ENET_IMPLEMENTATION
#include <enet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOST "127.0.0.1"
#define PORT 7000
#define BUFFER_SIZE 1000

#define HANDSHAKE "Handshake"

ENetAddress address = {0};
ENetHost *server = {0};
ENetEvent event = {0};
ENetPacket *packet = {0};

char buffer[BUFFER_SIZE] = {0};

bool online = true;

int main(int argc, char** argv)
{
  if(enet_initialize() != 0)
  {
    printf("Could not initalize enet\n");
    return 1;
  }

  address.host = ENET_HOST_ANY;
  address.port = PORT;

  server = enet_host_create(&address, 10, 2, 0, 0);

  if(server == NULL)
  {
    printf("could not start server\n");
    return 1;
  }

  printf("Server Online...\n");

  while(online)
  {
    while(enet_host_service(server, &event, 1000000) > 0)
    {
      switch(event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
          printf("New user connected !\n");
          break;
        
        case ENET_EVENT_TYPE_RECEIVE:
          if(event.peer->data == NULL)
          {
            event.peer->data = malloc(strlen((char*) event.packet->data)+1);
            event.peer->data = event.packet->data;
            packet = enet_packet_create(buffer, strlen(buffer)+1, 0);
            enet_host_broadcast(server, 1, packet);
          }
          else
          {
            for(int i=0; i < server->peerCount; i++)
            {
              if(&server->peers[i] != event.peer)
              {
                sprintf(buffer, "%s: %s\n", (char*) event.peer->data, event.packet->data);
                packet = enet_packet_create(buffer, strlen(buffer)+1, 0);
                enet_peer_send(&server->peers[i], 0, packet);
                enet_host_flush(server);
              }
            }
          }
          printf("packet receieved from %s: %s\n", event.peer->data, event.packet->data);
          break;

          case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s has disconnected\n", (char*) event.peer->data);
            sprintf(buffer, "%s has disconnected\n", (char*) event.peer->data);
            packet =  enet_packet_create(buffer, strlen(buffer)+1, 0);
            enet_host_broadcast(server, 1, packet);
            event.peer->data = NULL;
            break;
          
          case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            printf("%s has disconnected(timed out)\n", (char*) event.peer->data);
            sprintf(buffer, "%s has disconnected(timed out)\n", (char*) event.peer->data);
            packet = enet_packet_create(buffer, strlen(buffer)+1, 0);
            enet_host_broadcast(server->peers->host, 1, packet);
            event.peer = NULL;
            break;

          case ENET_EVENT_TYPE_NONE:
            printf("Tick Tock\n");
            break;
      }
    }
  }

  enet_host_destroy(server);
  enet_deinitialize();
  return 0;
}