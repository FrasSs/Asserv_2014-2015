#ifndef ORDRE
#define ORDRE

#include "struct_Commande.h"

extern Commande Ordre_actuel;
extern Commande Ordre_suivant;
extern Commande* Tab_ordre[];
extern uint8_t etat;
extern uint8_t new_etat;
extern int nb_ordre;

extern Commande I2CNewOrder;

void Actualisation_Ordre(int stack); // Mise à joure de Ordre_actuel et Ordre_suivant

void Actualisation_new_Ordre(int stack); // viens copier les ordres de l'I2C dans la pile et du compteur new_etat

void fct_Ordre_suivant(int * cond1,int *cond2,int stack,int stop); // mise à joure du compteur etat et effassement de l'ancien Ordre

#endif