
#include "../include/Ralentissement_free.h"


extern Commande Ordre_actuel;
extern Commande Ordre_suivant;


double Calcul_ralentissement(double V_Max,Commande position)
{
	double V_Min=0.0;
	double angle=0.0,angle1=0.0,angle2=0.0;
	Commande P1,P2;
	
	if(Ordre_suivant.Type==AVANCE_Free)
	{
	// Repère local reporté sur la position Ordre_actuel
		// ATTENTION le " - " n'est pas une erreur
		P1.X=-(position.X-Ordre_actuel.X);
		P1.Y=-(position.Y-Ordre_actuel.Y);
		
		P2.X=Ordre_suivant.X-Ordre_actuel.X;
		P2.Y=Ordre_suivant.Y-Ordre_actuel.Y;

		
		// angle à l'axe Z de la position actuelle et de la position 2 
		
		angle1=atan2(P1.Y,P1.X);
		angle1=Modulo_Pi(angle1);
		
		angle2=atan2(P2.Y,P2.X);
		angle2=Modulo_Pi(angle2);
		
		angle=angle1-angle2;
		angle=Modulo_Pi(angle);
		
		
		if (fabs(angle)<=(M_PI/2.0)) // seuil ou la n
		{
			V_Min=cos(angle)*V_Max; // par le cercle de rayon Vmax qui s'annule en PI/2
		}
	}
	
	return V_Min;
}