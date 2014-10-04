/*#if 0
#ifndef TRAITEMENTI2C_H
#define TRAITEMENTI2C_H

#define ORDER_SET_ACTION 'O'
#define ORDER_GET_STATE  'E'

typedef struct _i2c_packet {
	uint16_t Type;
	int16_t X;
	int16_t Y;
	int16_t Theta;
} i2c_packet;

void traitementI2C(unsigned char* msg,unsigned char size);
#endif

#endif*/


#ifndef TRAITEMENTI2C_H
#define TRAITEMENTI2C_H

#if 0
#define _GO_X_Y_THETA		0x10
#define _SET_POSITION_ASSER	0x13
#define _TURN				0x15
#define _WHERE_ARE_WE		0x17

#define _STOP_ABRUPTLY		0x20
#define _STOP				0x21
#define _RECALAGE			0x26


#define _ACTIV_ASSERV		0x36
#define _IS_BLOCKED			0x38

#define _INFO				0x40
#define _IS_MOVING			0x42
#define _FORWARD			0x44

#define _SET_CONFIG_PID		0x50
#define _SET_CONFIG_RAPPORT	0x51
#define _GO_TEST_PID		0x52
#define _SET_SEUIL_BLOCK	0x53
#define _GET_MAX_SEUIL		0x54
#define _RESET_MAX_SEUIL	0x55

#define _SET_V_A_D			0x60
#define _GET_V_A_D			0x61


#endif


#include "config.h"

typedef struct _i2c_packet {
	uint16_t Type;
	int16_t X;
	int16_t Y;
	int16_t Theta;
} i2c_packet;

void traitementI2C(unsigned char* msg,unsigned char size);
#endif
