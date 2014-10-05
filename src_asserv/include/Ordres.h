#ifndef ORDRE
#define ORDRE

#include "config.h"

void Actualisation_Ordre(void); // Mise à joure de Ordre_actuel et Ordre_suivant

void Actualisation_new_Ordre(void); // viens copier les ordres de l'I2C dans la pile et du compteur new_etat

void fct_Ordre_suivant(int * cond1,int *cond2); // mise à joure du compteur etat et effassement de l'ancien Ordre

#endif