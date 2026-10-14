#include "mp_networking.h"

uint8_t ReadByte(ENetPacket* packet, size_t* offset)
{
	if (*offset > packet->dataLength)
		return 0;
	uint8_t* ptr = (uint8_t*)packet->data;

	uint8_t data = ptr[(*offset)];

	*offset = *offset + 1;

	return data;
}

int16_t ReadShort(ENetPacket* packet, size_t* offset)
{
	if (*offset > packet->dataLength)
		return 0;

	uint8_t* data = (uint8_t*)packet->data;
	data += (*offset);

	*offset = (*offset) + 2;

	return *(int16_t*)data;
}

float ReadFloat(ENetPacket* packet, size_t* offset) // I feel so smart for figuring this out :] maybe not the cleanest but it works!
{
  if(*offset > packet->dataLength) return 0;

  float f = 0;

  uint8_t data[4];
  data[0] = (uint8_t)packet->data[*offset];
  data[1] = (uint8_t)packet->data[*offset+1];
  data[2] = (uint8_t)packet->data[*offset+2];
  data[3] = (uint8_t)packet->data[*offset+3];
  memcpy(&f, data, sizeof(data));

  *offset = (*offset) + 4;

  return f;
}