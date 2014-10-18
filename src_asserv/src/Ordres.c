
#include "../include/Ordres.h"

extern Commande Ordre_actuel;
extern Commande Ordre_suivant;
extern Commande* Tab_ordre[];
extern uint8_t etat;
extern int nb_ordre;
extern uint8_t I2CNewOrderFlag;


//////////////////////////////////////////////////////////////////////////
///Tab_ordre : c'est la pile d'ordre mise à jour par l'i2c////////////////
///Ordre_actuel : ordre en cours d'execution//////////////////////////////
///Ordre_suivant : ordre qui suivra celui en cours d'execution////////////
///////////////////il est utile pour prévoir la courbure de l'action//////
//////////////////////////////////////////////////////////////////////////

void Actualisation_Ordre() // mise à jour de Ordre_actuel et Ordre_suivant
{
	int etat_suiv=0;
	
	Ordre_actuel.Type=Tab_ordre[etat]->Type;
	Ordre_actuel.X=Tab_ordre[etat]->X;
	Ordre_actuel.Y=Tab_ordre[etat]->Y;
	Ordre_actuel.Theta=Tab_ordre[etat]->Theta;
	
	etat_suiv = ((etat+1)<STACK_SIZE) ? etat+1 : 0;

	Ordre_suivant.Type=Tab_ordre[etat_suiv]->Type;
	Ordre_suivant.X=Tab_ordre[etat_suiv]->X;
	Ordre_suivant.Y=Tab_ordre[etat_suiv]->Y;
	Ordre_suivant.Theta=Tab_ordre[etat_suiv]->Theta;
}



//////////////////////////////////////////////////////////////////////////
//fct_Ordre_suivant : fonction permettant de passer à l'ordre suivant/////
//////////////////////////////////////////////////////////////////////////

void Ordre_suivant_fct(int *cond1,int *cond2,int *demarrage)
{
	if (((*cond1==1)&&(*cond2==1)) || (*demarrage==1)) // si fin d'état
	{		
		if (nb_ordre)// incrément jusqu'à attendre new_etat et bloque sur le dernier ordre
		{
			// mise à jour des ordres de la pile sur Ordre_actuel et Ordre_suivant
			Actualisation_Ordre();
			
			DePile(); // Dépilement de la Pile
			
			*cond1=0;
			*cond2=0;			
		}
		
		
		if (*demarrage<5)
		{
			*demarrage+=1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// fct ordre suivant /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void Recep_Ordre_i2c(int *demarrage)
{
	if ((I2CNewOrderFlag==1) && (nb_ordre<STACK_SIZE))  // un nouvel ordre recu sur le bus I2C
	{
		// Mise a  jour du pointeur de la pile d'ordre pour pointe sur le dernier ordre rea§u

		Pile();
			
		if (*demarrage<5)
		{
			*demarrage+=1;
		}
			
		I2CNewOrderFlag = 0;
	}
}