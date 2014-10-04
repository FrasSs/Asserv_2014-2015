
#include "../include/Ordres.h"

//////////////////////////////////////////////////////////////////////////
///Tab_ordre : c'est la pile d'ordre mise à jour par l'i2c////////////////
///Ordre_actuel : ordre en cours d'execution//////////////////////////////
///Ordre_suivant : ordre qui suivra celui en cours d'execution////////////
///////////////////il est utile pour prévoir la courbure de l'action//////
//////////////////////////////////////////////////////////////////////////

void Actualisation_Ordre(int stack) // mise à jour de Ordre_actuel et Ordre_suivant
{
	int etat_suiv=0;
	
	Ordre_actuel.Type=Tab_ordre[etat]->Type;
	Ordre_actuel.X=Tab_ordre[etat]->X;
	Ordre_actuel.Y=Tab_ordre[etat]->Y;
	Ordre_actuel.Theta=Tab_ordre[etat]->Theta;
	
	etat_suiv = (etat<(stack-1)) ? etat+1 : 0;
	
	Ordre_suivant.Type=Tab_ordre[etat_suiv]->Type;
	Ordre_suivant.X=Tab_ordre[etat_suiv]->X;
	Ordre_suivant.Y=Tab_ordre[etat_suiv]->Y;
	Ordre_suivant.Theta=Tab_ordre[etat_suiv]->Theta;
}

//////////////////////////////////////////////////////////////////////////
//new_etat : numéro de la ligne dans Tab_ordre de l'ordre reçu en dernier/
//I2CNewOrder : nouvel ordre envoyé par l'I2C/////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Actualisation_new_Ordre(int stack) // sauvegarde de l'ordre dans la pile et incrémentation
{
	new_etat=(new_etat <(stack-1))? new_etat++:0; 
	
	//////////////////////////////////////////////////////////////////////////
	// convertion des ordes I2C en double ////////////////////////////////////
	// Enregistrement du nouvel ordre sur la pile.
	Tab_ordre[new_etat]->Type = I2CNewOrder.Type;
	Tab_ordre[new_etat]->X = (double)I2CNewOrder.X/* / 10.0*/;
	Tab_ordre[new_etat]->Y = (double)I2CNewOrder.Y/* / 10.0*/;
	Tab_ordre[new_etat]->Theta = (double)I2CNewOrder.Theta/* / 10000.0*/;
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
//fct_Ordre_suivant : fonction permettant de passer à l'ordre suivant/////
//////////////////////////////////////////////////////////////////////////

void fct_Ordre_suivant(int *cond1,int *cond2,int stack,int stop)
{
	if ((*cond1==1)&&(*cond2==1)) // si fin d'état
	{		
		if ((nb_ordre-1)>0)// incrément jusqu'à attendre new_etat
		{
			//////////////////////////////////////////////////////////////////////////
			//Effacement de l'ordre actuel////////////////////////////////////////////
			Tab_ordre[etat]->Type=stop;
			Tab_ordre[etat]->X=0;
			Tab_ordre[etat]->Y=0;
			Tab_ordre[etat]->Theta=0;
			*cond1=0;
			*cond2=0;
			//////////////////////////////////////////////////////////////////////////
			//Incrémentation du numéro de l'ordre à lire//////////////////////////////
			etat=(etat<(stack-1)) ? etat+1 : 0;
		}
		
		// mettre à jour le nombre d'ordre reçu (3 maxi) retour sur variable : nb_ordre
		nb_ordre_save();
				
		// mise à joure des ordres de la pile sur Ordre_actuel et Ordre_suivant
		Actualisation_Ordre(stack);
	}
}