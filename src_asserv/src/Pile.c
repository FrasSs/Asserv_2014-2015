
#include "../include/Pile.h"


extern Commande* Tab_ordre[];
extern uint8_t etat;
extern uint8_t new_etat;
extern uint8_t nb_ordre;
extern Commande I2CNewOrder;

//////////////////////////////////////////////////////////////////////////
//new_etat : numéro de la ligne dans Tab_ordre de l'ordre reçu en dernier/
//I2CNewOrder : nouvel ordre envoyé par l'I2C/////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Pile(void) // sauvegarde de l'ordre dans la pile et incrémentation
{
	//////////////////////////////////////////////////////////////////////////
	// convertion des ordes I2C en double ////////////////////////////////////
	// Enregistrement du nouvel ordre sur la pile.
	Tab_ordre[new_etat]->Type = I2CNewOrder.Type;
	Tab_ordre[new_etat]->X = (double)I2CNewOrder.X/* / 10.0*/;
	Tab_ordre[new_etat]->Y = (double)I2CNewOrder.Y/* / 10.0*/;
	Tab_ordre[new_etat]->Theta = (double)I2CNewOrder.Theta/* / 10000.0*/;
	//////////////////////////////////////////////////////////////////////////
	
	new_etat=((new_etat+1) <STACK_SIZE)? new_etat++:0; // pointe sur la dernière case vide
	
	// mettre à jour le nombre d'ordre reçu (3 maxi) retour sur variable : nb_ordre
	nb_ordre_pp();
}
//////////////////////////////////////////////////////////////////////////

void Erase_Pile(void)
{
	int eff_ordre;
	for(eff_ordre=0;(eff_ordre+1)<STACK_SIZE;eff_ordre++) // effassement de tous les ordres
	{
		Tab_ordre[eff_ordre]->Type=STOP;
		Tab_ordre[eff_ordre]->X=0;
		Tab_ordre[eff_ordre]->Y=0;
		Tab_ordre[eff_ordre]->Theta=0;
	}
	
	// mettre à jour le nombre d'ordre reçu (3 maxi) retour sur variable : nb_ordre
	new_etat=0;
	etat=0;
	nb_ordre=0;
}

void DePile(void) 
{
	
	Tab_ordre[etat]->Type=STOP;
	Tab_ordre[etat]->X=0;
	Tab_ordre[etat]->Y=0;
	Tab_ordre[etat]->Theta=0;
	//////////////////////////////////////////////////////////////////////////
	//Incrémentation du numéro de l'ordre à lire//////////////////////////////
	etat=((etat+1)<STACK_SIZE) ? etat+1 : 0;
	
	// mettre à jour le nombre d'ordre reçu (3 maxi) retour sur variable : nb_ordre
	nb_ordre_mm();
}