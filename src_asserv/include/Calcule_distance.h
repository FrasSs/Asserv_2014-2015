
#ifndef TRAPEZE_POLAIRE_H_
#define TRAPEZE_POLAIRE_H_


#include <math.h>
#include "Modulo_pi.h"
#include "config.h"

//////////////////////////////////////////////////////////////////////////
// Variable extern

extern Commande position;
extern Commande Ordre_actuel;
extern double distance_restante;
extern double angle_restant;
extern int positionnement_precis_U; // variable d'arrivé en zone
extern int positionnement_precis_T; // variable d'arrivé en zone

//////////////////////////////////////////////////////////////////////////


void Calcule_deplacement(int Action); // fonction de mise à jour des commande robot (calculé en fonction de l'ordre et de la position du robot)



#endif /* TRAPEZE_POLAIRE_H_ */ 