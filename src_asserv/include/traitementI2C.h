#ifndef TRAITEMENTI2C_H
#define TRAITEMENTI2C_H

#include "config.h"
#include "../hardware/i2c.h"
#include "Mouvement_Robot.h"



typedef struct _i2c_packet {
	uint16_t Type;
	int16_t X;
	int16_t Y;
	int16_t Theta;
} i2c_packet;

void traitementI2C(unsigned char* msg,unsigned char size);
#endif
