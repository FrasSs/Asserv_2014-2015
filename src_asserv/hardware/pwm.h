/*
 * pwm.h
 *
 *  Created on: 12 oct. 2009
 *      Author: damunix
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

#define ROUE_DROITE 0
#define ROUE_GAUCHE 1

#define SENSD PIN3_bm
#define SENSG PIN2_bm

void pwm_init();

// -100 < val < 100
void pwm_set(unsigned char roue,double val);

#endif /* PWM_H_ */
