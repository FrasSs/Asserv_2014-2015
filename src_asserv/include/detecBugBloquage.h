#ifndef DETECT_BB
#define DETECT_BB

#include "config.h"
#include "STOP_Force.h"

extern Commande Ordre_actuel;
extern double moteur_G; // variable Commande moteur
extern double moteur_D; // variable Commande moteur

void detecBugBloquage(void); // mise à 1 de BugBloquage une fois une erreur capté
void acknowledge_bloquage(void); // mise à 0 de BugBloquage

#endif
