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