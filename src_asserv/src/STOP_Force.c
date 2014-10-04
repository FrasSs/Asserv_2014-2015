
#include "../include/STOP_Force.h"


void fct_STOP_F(int stop)
{
	
	int eff_ordre=0;		// Compteur pour l'ordre STOP_F pour l'effacement des ordres
	
	for(eff_ordre=0;Tab_ordre[eff_ordre+1]->Type==0;eff_ordre++); // compt stack
	Actualisation_Ordre(eff_ordre);
	
	etat=0;
	new_etat=0;
	
	moteur_G = 0 ;
	moteur_D = 0 ;
	Vitesse_C_T=0;
	Vitesse_C_U=0;
	distance_restante=0;
	angle_restant=0;
		
	// passage à l'orde suivant si un nouveau à été envoyé
	positionnement_precis_U=0;
	positionnement_precis_T=0;
				
	nb_ordre_save(); // mettre à jour le nombre d'ordre reçu (3 maxi) retour sur variable : nb_ordre
	acknowledge_bloquage();
				
	for(eff_ordre=0;Tab_ordre[eff_ordre+1]->Type==0;eff_ordre++) // effassement de tous les ordres
	{
		Tab_ordre[eff_ordre]->Type=stop;
		Tab_ordre[eff_ordre]->X=0;
		Tab_ordre[eff_ordre]->Y=0;
		Tab_ordre[eff_ordre]->Theta=0;
	}
}