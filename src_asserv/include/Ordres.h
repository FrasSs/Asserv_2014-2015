#ifndef ORDRE
#define ORDRE

#include "Pile.h"
#include "config.h"
#include "traitementI2C.h"

void Actualisation_Ordre(void); // Mise à joure de Ordre_actuel et Ordre_suivant

void Ordre_suivant_fct(int * cond1,int *cond2,int * demarrage); // mise à joure du compteur etat et effassement de l'ancien Ordre

void Recep_Ordre_i2c(int *demarrage);

#endif