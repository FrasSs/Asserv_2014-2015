/*
 * codeur.c
 *
 *  Created on: 5 oct. 2009
 *      Author: damunix
 */

#include "../include/avr_compiler.h"
#include "codeur.h"
#include "drivers/TC_driver.h"
#include "drivers/port_driver.h"
#include "drivers/qdec_driver.h"
#include <math.h>

//void _init(_codeur * codeur)
//{
	//// Codeur est un pointeur valide
 	//if(codeur != NULL)
	//{
 		//codeur_raz(codeur);
 		//codeur->sens = 0;				// Avance par defaut (pas d'importance , juste pour initialisation)
//
		//// permet de faire les calculs necessaires a l'initialisation, et pas pendant l'execution du code
		//codeur->coeffDep = ( M_PI * codeur->diaRoue) / (codeur->nbPasCodeur);
		//codeur->coeffVit = 1 / PERIODE;
 	//}
//}
//void codeur_init(_codeur * codeur1)
//{
//
	//_init(codeur1);
//}
void codeurs_init()
{
	uint16_t lineCount = 1024;
	QDEC_Total_Setup(&PORTD,                    /*PORT_t * qPort*/
	                 0,                         /*uint8_t qPin*/
	                 false,                     /*bool invIO*/
	                 0,                         /*uint8_t qEvMux*/
	                 EVSYS_CHMUX_PORTD_PIN0_gc, /*EVSYS_CHMUX_t qPinInput*/
	                 false,                     /*bool useIndex*/
	                 EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
	                 &TCC0,                     /*TC0_t * qTimer*/
	                 TC_EVSEL_CH0_gc,           /*TC_EVSEL_t qEventChannel*/
	                 lineCount);                /*uint8_t lineCount*/
	TC_Restart( &TCC0 );
	TC_SetCount(&TCC0,10000);
	QDEC_Total_Setup(&PORTD,                    /*PORT_t * qPort*/
	                 2,                         /*uint8_t qPin*/
	                 false,                     /*bool invIO*/
	                 2,                         /*uint8_t qEvMux*/
	                 EVSYS_CHMUX_PORTD_PIN2_gc, /*EVSYS_CHMUX_t qPinInput*/
	                 false,                     /*bool useIndex*/
	                 EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
	                 &TCD0,                     /*TC0_t * qTimer*/
	                 TC_EVSEL_CH2_gc,           /*TC_EVSEL_t qEventChannel*/
	                 lineCount);                /*uint8_t lineCount*/
	TC_Restart( &TCD0 );
	TC_SetCount(&TCD0,10000);
}

//void codeurs_acquire()
//{
	//cli(); // Disable interruptions. The acquisition of encoders must be atomic.
//
	//robot.codD.pasCourant = TCC0.CNT; // Acquire encoder
	//robot.codG.pasCourant = TCD0.CNT;
//
	//// Timer restart.
	//TCC0.CTRLFSET = TC_CMD_RESTART_gc;
	//TCC0.CNT = 10000;
//
	//TCD0.CTRLFSET = TC_CMD_RESTART_gc;
	//TCD0.CNT = 10000;
//
	//sei(); // enable information
//}
//void codeur_update(_codeur * codeur)
//{
///*	cli();
   	//int16_t pas = qTimer->CNT;
	//TC_Restart( qTimer );
	//TC_SetCount(qTimer,10000);
	//sei();*
	//pas-=10000;
	////pas/=4;
	//pas*=codeur->sens;  // bit de sens pour avoir un d�placement positif en avan�ant
	//codeur->deltaPas = pas;*/
	//codeur->pasCourant -= 10000;
	//codeur->pasCourant *= codeur->sens;
//
	//codeur->deltaPas = codeur->pasCourant;
//
	//codeur->nbPasTot += codeur->deltaPas;
//
	//codeur->deltaDepl = (double)(codeur->deltaPas) * (double)codeur->coeffDep;			// Mise a jour deplacement en m
	//codeur->distTot += codeur->deltaDepl;
//
	//codeur->vitActuel = (double)codeur->deltaDepl * ((double)codeur->coeffVit);		// Mise a jour vitesse de rotation
//
//}
//
//void codeur_raz(_codeur * codeur)
//{
	//codeur->deltaPas=0;
	//codeur->deltaDepl=0;
	//codeur->vitActuel=0;
	//codeur->nbPasTot=0;
//}
//
//


