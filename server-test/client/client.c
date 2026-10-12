#define ENET_IMPLEMENTATION
#include <enet.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define HOST "127.0.0.1"
#define PORT 7000
#define BUFFER_SIZE 1000

#define CONNECTED "connected"
#define HANDSHAKE "Handshake"

char buffer[BUFFER_SIZE] = {0};

ENetHost *client = {0};
ENetAddress address = {0};
ENetEvent event = {0};
ENetPeer *peer = {0};
ENetPacket *packet = {0};

int main(int argc, char** argv)
{
  int connected = 0;

  if(argc != 2)
  {
    printf("Usage: clientname\n");
    return 1;
  }

  if(enet_initialize() != 0)
  {
    printf("could not initialize ENet\n");
    return 1;
  }

  client = enet_host_create(NULL, 1, 2, 5760/8, 1440/8);

  if(client == NULL)
  {
    printf("Could not create client\n");
    return 1;
  }

  enet_address_set_host(&address, HOST);
  address.port = PORT;

  peer = enet_host_connect(client, &address, 2, 0);

  if(peer = NULL)
  {
    printf("Could not connect to server\n");
    return 1;
  }

  if(enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
  {
    printf("Connection to %s succeeded\n", HOST);
    connected++;

    strncpy(buffer, argv[1], BUFFER_SIZE);

    packet = enet_packet_create(buffer, strlen(buffer)+1, ENET_PACKET_FLAG_RELIABLE);
 
    enet_peer_send(event.peer, 0, packet);
  }
  else
  {
    enet_peer_reset(peer);
    printf("Could not connect to %s\n", HOST);
    return 0;
  }

  while(1)
  {
    while(enet_host_service(client, &event, 1000) > 0)
    {
      switch(event.type)
      {
        case ENET_EVENT_TYPE_RECEIVE:
          puts((char*) event.packet->data);
          printf("%s\n", event.packet->data);
          if(strcmp(event.packet->data, HANDSHAKE))
          {
            packet = enet_packet_create(CONNECTED, strlen(CONNECTED)+1, ENET_PACKET_FLAG_RELIABLE);
          }
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          connected=0;
          printf("You have been disconnected\n");
          return 2;
      }
    }
    if(connected == true)
    {
      char hello[100] = "bla bla bla";
      strcpy(buffer, hello);

      if(strncmp("q", buffer, BUFFER_SIZE) == 0)
      {
        connected = false;
        enet_peer_disconnect(client->peers, 0);
        continue;
      }
      packet = enet_packet_create(buffer, strlen(buffer)+1, ENET_PACKET_FLAG_RELIABLE);
      enet_peer_send(client->peers, 0, packet);
    }
  }

  enet_host_destroy(client);
  enet_deinitialize();
  return 0;
}