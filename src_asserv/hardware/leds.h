/*
 * leds.h
 *
 *  Created on: 5 oct. 2009
 *      Author: damunix
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <avr/io.h>

#define LED1 PIN0_bm   //carte sandy(2013) : orange
#define LED2 PIN1_bm   //carte sandy(2013) : rouge
#define LED3 PIN2_bm   //carte sandy(2013) : vert
#define LED4 PIN3_bm   //carte sandy(2013) : orange

#define leds_on(LED) ( PORTA.OUTSET = (LED) )
#define leds_off(LED) ( PORTA.OUTCLR = (LED) )
#define leds_toggle(LED) ( PORTA.OUTTGL = (LED) )

void leds_configure();


#endif /* LEDS_H_ */
