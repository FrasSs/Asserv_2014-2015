
#ifndef STRUCT_COMMANDE_H_
#define STRUCT_COMMANDE_H_

#include <stdint.h>

typedef struct _Commande // structure de base pour tous nos positionnement et commandes
{
	uint8_t Type ;
	double X ;
	double Y ;
	double Theta ;
} Commande;


#endif /* STRUCT_COMMANDE_H_ */