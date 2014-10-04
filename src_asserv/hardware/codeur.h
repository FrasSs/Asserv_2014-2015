/*
 * codeur.h
 *
 *  Created on: 12 oct. 2009
 *      Author: damunix
 */

#ifndef CODEUR_H_
#define CODEUR_H_


 /*
  * Structure d'un codeur de roue (capteurs)
  */
typedef struct{
	// delta codeur entre deux maj d'asserv
	int pasCourant;			// Nombre de pas courants
	int deltaPas;       	// delta pas entre deux p?riodes d'asserv

	double deltaDepl;		// Deplacement en mm entre deux lecture des capteurs
	double vitActuel;	    // Vitesse de deplacement

	int32_t nbPasTot;		// nombre de pas au total fait par le codeur
	double distTot;			// distance totale parcourue par le codeur

	// parametres
	int sens;				// Sens du deplacement entre deux asservissement des capteurs
	char type;				// moteur ou roue folle
	int nbPasCodeur;   	 	// nombre de pas codeurs
	double diaRoue;     	// diametre roue en m
	double coeffDep;		// multiplication faite a l'initialisation du codeur
	double coeffVit;
	//TC0_t* timer;	//timer associ� au module de d�codage
} _codeur;

#define PIN_IS_HIGH(PORT,PIN) ((PORT>>PIN) & 0x01)

 /*
  * Fonction d'initialisation d'une structure codeur
  */
void codeurs_init();

void codeur_update(_codeur * codeur);
void codeurs_acquire();
 /*
  * Fonction de reinitialisation des codeurs
  */
void codeur_raz(_codeur * codeur);

#endif /* CODEUR_H_ */
