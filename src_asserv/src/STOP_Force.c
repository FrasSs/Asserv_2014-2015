
#include "../include/STOP_Force.h"


extern double moteur_G;
extern double moteur_D;
extern double Vitesse_C_U;
extern double Vitesse_C_T;
extern double distance_restante;
extern double angle_restant;
extern int positionnement_precis_U;
extern int positionnement_precis_T;

void fct_STOP_F(void)
{
	moteur_G = 0 ;
	moteur_D = 0 ;
	Vitesse_C_T=0;
	Vitesse_C_U=0;
	distance_restante=0;
	angle_restant=0;
		
	// passage � l'orde suivant si un nouveau � �t� envoy�
	positionnement_precis_U=0;
	positionnement_precis_T=0;
				
	acknowledge_bloquage(); //??????
	
	Erase_Pile();
	Actualisation_Ordre();
}