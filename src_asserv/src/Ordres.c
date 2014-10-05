
#include "../include/Ordres.h"

extern Commande Ordre_actuel;
extern Commande Ordre_suivant;
extern Commande* Tab_ordre[];
extern uint8_t etat;
extern int nb_ordre;


//////////////////////////////////////////////////////////////////////////
///Tab_ordre : c'est la pile d'ordre mise � jour par l'i2c////////////////
///Ordre_actuel : ordre en cours d'execution//////////////////////////////
///Ordre_suivant : ordre qui suivra celui en cours d'execution////////////
///////////////////il est utile pour pr�voir la courbure de l'action//////
//////////////////////////////////////////////////////////////////////////

void Actualisation_Ordre() // mise � jour de Ordre_actuel et Ordre_suivant
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
//fct_Ordre_suivant : fonction permettant de passer � l'ordre suivant/////
//////////////////////////////////////////////////////////////////////////

void fct_Ordre_suivant(int *cond1,int *cond2)
{
	if ((*cond1==1)&&(*cond2==1)) // si fin d'�tat
	{		
		if (nb_ordre)// incr�ment jusqu'� attendre new_etat et bloque sur le dernier ordre
		{
			DePile(); // D�pilement de la Pile
			
			// mise � jour des ordres de la pile sur Ordre_actuel et Ordre_suivant
			Actualisation_Ordre();
			
			*cond1=0;
			*cond2=0;			
		}
	}
}