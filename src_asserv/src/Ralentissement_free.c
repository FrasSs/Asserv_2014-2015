/*
 * config.c
 *
 * Created: 29/09/2012 17:05:55
 *
 */ 

#include "../include/Ralentissement_free.h"


extern Commande Ordre_actuel;
extern Commande Ordre_suivant;


double Calcul_ralentissement(double Vitesse, double V_Max,Commande position)
{
	double V_Min=0.0;
	double angle=0.0,angle1=0.0,angle2=0.0;
	Commande P1;
	
	if(Ordre_suivant.Type==AVANCE_Free)
	{
	// Repère local sur la position 1 (le premier point d'arrivé)
		position.X=position.X-Ordre_actuel.X; 
		position.Y=position.Y-Ordre_actuel.Y;
		
		P1.X=Ordre_suivant.X-Ordre_actuel.X;
		P1.Y=Ordre_suivant.Y-Ordre_actuel.Y;

		
		// angle à l'axe Z de la position actuelle et de la position 2 
		angle2=atan2(P1.Y,P1.X);
		angle1=atan2(position.Y,position.X);
		
		angle=fmod(angle1-angle2,2.0*M_PI);
		
		if (angle>=M_PI)
		{
			angle-=2.0*M_PI;
		}
		
		if (angle<=(-M_PI))
		{
			angle+=2.0*M_PI;
		}
		
		angle=M_PI-fabs(angle);
		
		if (fabs(angle)<=(M_PI/2.0))
		{
			//V_Min=(-exp(fabs(angle)*4.0))+V_Max; //par exponeciel
			V_Min=cos(angle)*V_Max; // par le cercle de rayon Vmax qui s'annule en PI/2
			if (V_Min<0)
			{
				V_Min=0;
			}
			
			if (V_Min>(Vitesse-1))
			{
				V_Min=Vitesse;
			}
		}
		else
		{
			V_Min=0;
		}
	}
	else
	{
		V_Min=0.0;
	}
	return V_Min;
}