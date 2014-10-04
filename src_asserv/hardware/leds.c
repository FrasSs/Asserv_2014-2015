/*
 * leds.c
 *
 *  Created on: 5 oct. 2009
 *      Author: damunix
 */

#include <avr/io.h>

void leds_configure()
{
	PORTA.DIR |= 0x0F; //4 leds
	PORTA.OUT = 0x00; // tout le port à 0
}

