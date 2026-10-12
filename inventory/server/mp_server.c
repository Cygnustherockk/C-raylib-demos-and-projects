#define ENET_IMPLEMENTATION
#include <enet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/mp_networking.h"

#define HOST "127.0.0.1"
#define PORT 7000
#define BUFFER_SIZE 1000


#define HANDSHAKE "Handshake"

ENetAddress address = {0};
ENetHost *server = {0};
ENetEvent event = {0};
ENetPacket *packet = {0};

bool online = true;

int current_player_count = 0;


typedef struct PlayerData{
  bool active;
  bool validPosition;
  ENetPeer *peer;

  int16_t X;
  int16_t Y;
  int16_t Z;

  int16_t DX;
  int16_t DY;
  int16_t DZ;
}PlayerData;

PlayerData players[MAX_PLAYER_COUNT] = {0};

int GetPlayerId(ENetPeer* peer)
{
	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		if (players[i].active && players[i].peer == peer)
			return i;
	}
	return -1;
}

void SendToAllBut(ENetPacket* packet, int exceptPlayerId)
{
	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		if (!players[i].active || i == exceptPlayerId)
			continue;

		enet_peer_send(players[i].peer, 0, packet);
	}
}


int main()
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
    while(enet_host_service(server, &event, 1000) > 0)
    {
      switch(event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
        {
          current_player_count++;
          printf("%d\n", current_player_count);
          int playerID = 0;

          for(; playerID < MAX_PLAYER_COUNT; playerID++)
          {
            if(!players[playerID].active)
            {
              break;
            }
          }

          if(playerID == MAX_PLAYER_COUNT)
          {
            printf("Too many players, %d\n", playerID);
            enet_peer_disconnect(event.peer, 0);
            break;
          }
          players[playerID].active = true;
          players[playerID].validPosition = false;
          players[playerID].peer = event.peer;

          uint8_t buffer[2] = {0};

          buffer[0] = (uint8_t)accept_player;
          buffer[1] = (uint8_t)playerID;

          ENetPacket *packet = enet_packet_create(buffer, 2, ENET_PACKET_FLAG_RELIABLE);
          enet_peer_send(event.peer, 0, packet);

          for(int i = 0; i < MAX_PLAYER_COUNT; i++)
          {
            if(i == playerID || !players[i].validPosition)continue;
            uint8_t addBuffer[14] = {0};
            addBuffer[0] = (uint8_t)add_player;
            addBuffer[1] = (uint8_t)i;
            *(uint16_t*)(addBuffer + 2) = (uint16_t)players[i].X;
            *(uint16_t*)(addBuffer + 4) = (uint16_t)players[i].Y;
            *(uint16_t*)(addBuffer + 6) = (uint16_t)players[i].Z;
            *(uint16_t*)(addBuffer + 8) = (uint16_t)players[i].DX;
            *(uint16_t*)(addBuffer + 10) = (uint16_t)players[i].DY;
            *(uint16_t*)(addBuffer + 12) = (uint16_t)players[i].DZ;

            packet = enet_packet_create(addBuffer, 14, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(event.peer, 0, packet);
          }
        }
          break;
        
        case ENET_EVENT_TYPE_RECEIVE:
        {
          int playerID = GetPlayerId(event.peer);
          if(playerID == -1)
          {
            printf("PlayerID is -1\n");
            enet_peer_disconnect(event.peer, 0);
            break;
          }
          size_t offset = 0;
          
          NetworkCommands command = ReadByte(event.packet, &offset);

          if(command == update_input)
          {
            players[playerID].X = ReadShort(event.packet, &offset);
            players[playerID].Y = ReadShort(event.packet, &offset);
            players[playerID].Z = ReadShort(event.packet, &offset);
            players[playerID].DX = ReadShort(event.packet, &offset);
            players[playerID].DY = ReadShort(event.packet, &offset);
            players[playerID].DZ = ReadShort(event.packet, &offset);

            NetworkCommands outbound_command = update_player;

            if(!players[playerID].validPosition) outbound_command = add_player;

            players[playerID].validPosition = true;

						uint8_t buffer[14] = { 0 };
						buffer[0] = (uint8_t)outbound_command;
						buffer[1] = (uint8_t)playerID;
						*(int16_t*)(buffer + 2) = (int16_t)players[playerID].X;
						*(int16_t*)(buffer + 4) = (int16_t)players[playerID].Y;
						*(int16_t*)(buffer + 6) = (int16_t)players[playerID].Z;
						*(int16_t*)(buffer + 8) = (int16_t)players[playerID].DX;
						*(int16_t*)(buffer + 10) = (int16_t)players[playerID].DY;
						*(int16_t*)(buffer + 12) = (int16_t)players[playerID].DZ;

						ENetPacket* packet = enet_packet_create(buffer, 14, ENET_PACKET_FLAG_RELIABLE);
						SendToAllBut(packet, playerID);
          }
        
          enet_packet_destroy(event.packet);
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
        {
          current_player_count--;
          if(current_player_count <= 0)
          {
            online = false;
          }

          int playerID = GetPlayerId(event.peer);
          if(playerID == -1) break;

          players[playerID].active = false;
          players[playerID].peer = NULL;

          uint8_t buffer[2] = {0};
          buffer[0] = (uint8_t)remove_player;
          buffer[1] = (uint8_t)playerID;

          ENetPacket *packet = enet_packet_create(buffer, 2, ENET_PACKET_FLAG_RELIABLE);
          SendToAllBut(packet, -1);
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
        {
          current_player_count--;
          if(current_player_count == 0)
          {
            online = false;
          }

          int playerID = GetPlayerId(event.peer);
          if(playerID == -1) break;

          players[playerID].active = false;
          players[playerID].peer = NULL;

          uint8_t buffer[2] = {0};
          buffer[0] = (uint8_t)remove_player;
          buffer[1] = (uint8_t)playerID;

          ENetPacket *packet = enet_packet_create(buffer, 2, ENET_PACKET_FLAG_RELIABLE);
          SendToAllBut(packet, -1);
          break;
        }
          case ENET_EVENT_TYPE_NONE:
            break;
      }
    }
  }

  enet_host_destroy(server);
  enet_deinitialize();
  return 0;
}