﻿
#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 32000000UL

#include "../hardware/avr_compiler.h"
#include "../hardware/drivers/port_driver.h"
#include "../hardware/drivers/clksys_driver.h"
#include "../hardware/drivers/pmic_driver.h"
#include "../hardware/codeur.h"
#include "../hardware/drivers/TC_driver.h"

#include <util/delay.h>
#include <stdint.h>
#include <math.h>

	#include "../hardware/pwm.h"
#include "../hardware/leds.h"
	#include "../hardware/i2c.h"

	#include "struct_Commande.h"
	#include "Lecture_trapeze.h"

	#include "Calcule_distance.h"
	#include "detecBugBloquage.h"

#include "Modulo_pi.h"
#include "Mouvement_Robot.h"
#include "Nombre_ordre_enregistre.h"
	#include "Odometrie.h"
#include "Ordres.h"
	#include "PI_Asservissement.h"
	#include "Fifo.h"
	#include "Ralentissement_free.h"
#include "STOP_Force.h"
#include "traitementI2C.h"


//////////////////////////////////////////////////////////////////////////
//Gros Robot//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Odométrie
#define ANTRAXE_D		132.9075113   // en mm
#define ANTRAXE_G		132.2987292   // en mm
#define INC_CODEUR_D	0.0049870979 // distance en mm pour un incrément
#define INC_CODEUR_G	0.0049809016 // distance en mm pour un incrément
#define Sens_codeur_D	-1
#define Sens_codeur_G	1

//PI
#define Ki_U 0//0.2
#define Kp_U 1//0.8

#define Ki_T 0.005//0.4
#define Kp_T 1.2

//Commande moteur
#define Sens_mot_D -1
#define Sens_mot_G -1

#define cablage_mot_D 1
#define cablage_mot_G 0
//////////////////////////////////////////////////////////////////////////
//Petit Robot/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Odométrie

/*#define ANTRAXE_D   84.5713460281   // en mm
#define ANTRAXE_G   87.5306305075   // en mm
#define INC_CODEUR_D  0.022926006// distance en mm
#define INC_CODEUR_G  0.0234019996// distance en mm
#define Sens_codeur_D -1
#define Sens_codeur_G 1

//PI
#define Ki_U 0.2
#define Kp_U 0.8
#define Ki_T 0.1
#define Kp_T 0.6

//Commande moteur
#define Sens_mot_D -1
#define Sens_mot_G 1

#define cablage_mot_D 0
#define cablage_mot_G 1
*/
//////////////////////////////////////////////////////////////////////////
// Ordre

// Ce sont les différents états du robot
#define STOP_F		1
#define AVANCE      2
#define TOURNE      3
#define STOP		4
#define POSITION_W	5
#define AVANCE_Free	6
#define POSITION_R	7
#define BLOQUE		8
#define CERCLE		9

#define STATUS_ROB	80

#define ORDRE_RECU_MAIS_PAS_ENCORE_TRAITE	20 //pour i2c


//////////////////////////////////////////////////////////////////////////
//Test

#define Rien 0
#define Brute_force 1
#define Test_codeuse 2
#define Commande_mot 3

//////////////////////////////////////////////////////////////////////////
// Pile

#define STACK_SIZE 4 // taille de la pile utilisé

//////////////////////////////////////////////////////////////////////////

int init(void);


// fct init_main /////////////////////////////////////////////////////////

void Init_codeurs(int *codeur_d,int *codeur_g); // fonction de mise à 1000 des codeuses et redémarrage du compteur

//////////////////////////////////////////////////////////////////////////


#endif /* CONFIG_H_ */