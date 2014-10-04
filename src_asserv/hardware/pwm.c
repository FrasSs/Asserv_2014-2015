/*
 * pwm.c
 *
 *  Created on: 12 oct. 2009
 *      Author: damunix
 */
#include <math.h>
#include "pwm.h"
#include "avr_compiler.h"
#include "drivers/TC_driver.h"
#define PWM_PERIODE 0x0300

void pwm_init()
{
	PORTC.DIR = PIN4_bm | PIN5_bm | SENSD | SENSG;
	//PORTA.DIR |= PIN5_bm | PIN6_bm;
	/* Set the TC period. */
	TC_SetPeriod( &TCC1, PWM_PERIODE );

	/* Configure the TC for single slope mode. */
	TC1_ConfigWGM( &TCC1, TC_WGMODE_SS_gc );

	/* Enable Compare channel A. */
	TC1_EnableCCChannels( &TCC1, TC1_CCAEN_bm );

	/* Enable Compare channel B. */
	TC1_EnableCCChannels( &TCC1, TC1_CCBEN_bm );

	/* Start timer by selecting a clock source. */
	TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV1_gc );

}

void pwm_set(unsigned char roue,double val)
{
	if(val>90)
		val = 90;

	if(val<-90)
		val = -90;

	val = val*PWM_PERIODE/100;

	/* Output new compare value. */
	if (roue == ROUE_DROITE)
	{
		TC_SetCompareB( &TCC1, abs(val) );
		if(val<0)
		{
			PORTC.OUTSET = SENSD;
		}
		else
		{
			PORTC.OUTCLR = SENSD;
		}
	}
	else
	{
		TC_SetCompareA(&TCC1, abs(val));
		if(val>0)
		{
			PORTC.OUTCLR = SENSG;
		}
		else
		{
			PORTC.OUTSET = SENSG;
		}
	}

	do {
		/*  Wait for the new compare value to be latched
		 *  from CCABUF[H:L] to CCA[H:L]. This happens at
		 *  TC overflow (UPDATE ).
		 */
	} while( TC_GetOverflowFlag( &TCC1) == 0 );

	/* Clear overflow flag. */
	TC_ClearOverflowFlag( &TCC1 );
}
