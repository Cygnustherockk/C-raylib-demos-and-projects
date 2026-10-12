#ifndef _MP_NETWORKING_H_
#define _MP_NETWORKING_H_

#define MAX_PLAYER_COUNT 10

typedef enum{
  accept_player = 1,
  add_player = 2,
  remove_player = 3,
  update_player = 4,
  update_input = 5
}NetworkCommands;

#if (_WIN32_WINNT < 0x0601)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#ifndef ENET_IMPLEMENTATION
#include <enet.h>
#endif

uint8_t ReadByte(ENetPacket* packet, size_t* offset);
int16_t ReadShort(ENetPacket* packet, size_t* offset);
#endif