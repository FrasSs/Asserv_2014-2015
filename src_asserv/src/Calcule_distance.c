
#include "../include/Calcule_distance.h"


void Calcule_deplacement(int action)
{
	int distance_restante_reel=0;
	
	// calcule repère ////////////////////////////////////////////////////////
	
	double	X_Bp=0,Y_Bp=0,Theta_B=0,Theta_Bp=0,	// repère fixe au robot mais parallèle à celui du terrain
			Y_Bpp=0;/*X_Bpp=0,*/				// repère fixe et orienté sur le robot
	
	//////////////////////////////////////////////////////////////////////////
	// Distance signé en fonction de la direction/////////////////////////////
	
	//calcule pour déterminer si le point d'arrivé est derrière le robot (négative en y sur le repère fixe au robot) 
	X_Bp=(Ordre_actuel.X)-position.X;
	Y_Bp=(Ordre_actuel.Y)-position.Y;
	
	distance_restante = sqrt((X_Bp)*(X_Bp) + (Y_Bp)*(Y_Bp) );
	
	Theta_B=atan2(X_Bp,Y_Bp);	// calcule de l'angle pour la position demandé sur le repère fixe au robot(mais non orienté, parallèle au repère de base du terrain).
	Theta_B=Modulo_Pi(Theta_B);
		
	Theta_Bp=Theta_B-position.Theta;	// calcule de l'angle  avec l'orientation réel du robot
	Theta_Bp=Modulo_Pi(Theta_Bp);
	
	Y_Bpp=distance_restante*cos(Theta_Bp); // coordonné du point arrivé sur le repère fixe au robot
	//X_Bpp=-distance_restante*sin(Theta_Bp); // coordonné du point arrivé sur le repère fixe au robot	
	
	//////////////////////////////////////////////////////////////////////////
	// Calcule comportement déplacement U ////////////////////////////////////
	// On considère que la distance à parcourir est celle que le robot à sur 
	// son axe Y
	
	distance_restante_reel=distance_restante; // par phytagor
	distance_restante= Y_Bpp; // deplacement restant ==> uniquement lorsque une distance apparait en Y(dans le repère fixe au robot)
	
	
	//////////////////////////////////////////////////////////////////////////
	// Calcule comportement angulaire ////////////////////////////////////////
	
	
	switch (action)
	{
		case AVANCE:
		{
			//////////////////////////////////////////////////////////////////////////
			// Phase d'approche 2 : se caler sur l'angle commandé par I2C
			if (distance_restante_reel<20) // phase d'aproche final (positionnement plus précis en U et mise en position du Theta ordonné)
			{
				angle_restant=Ordre_actuel.Theta-position.Theta;// angle à parcourir pour arriver à l'angle commendé sur le repère fixe au robot
			}
			// Phase d'approche 1 : se caler son cap sur le point demandé
			else
			{
				angle_restant=Theta_Bp;// angle à parcourir pour arriver à l'angle désiré sur le repère fixe au robot
			}
			
			//////////////////////////////////////////////////////////////////////////
			// dernière corection pour fixé la variation de Théta (modulo)////////////
			//angle optimale en passant les angles sur -PI et PI//////////////////////
			
			angle_restant=Modulo_Pi(angle_restant);// application de la correction
			
			//////////////////////////////////////////////////////////////////////////
			// critères des zones d'arrivées /////////////////////////////////////////
			
			if(fabs(angle_restant)<(M_PI/100.0)) // plage d'arrèt de la progression en Theta (zone ou on considère être arrivé en Theta)
			{
				positionnement_precis_T=1;
			}
			else
			{
				positionnement_precis_T=0;
			}
			
			if(fabs(distance_restante_reel)<10.0) // plage d'arrèt de la progression en U quand le robot est à la position x et y demandée
			{
				positionnement_precis_U= 1;
			}
			else
			{
				positionnement_precis_U=0;
			}
			//////////////////////////////////////////////////////////////////////////
			
			break;
		}
		
#if 1		
		case AVANCE_Free:
		{
			
			//////////////////////////////////////////////////////////////////////////
			// Correction direction par avance
			if (distance_restante>0)
			{
				angle_restant = fmod(Theta_Bp,2.0*M_PI); // angle à parcourir pour arriver à l'angle désiré sur le repère fixe au robot
			}
			// Correction direction par recule
			else
			{
				angle_restant = fmod(Theta_Bp,2.0*M_PI)-M_PI; // angle à parcourir pour arriver à l'angle désiré sur le repère fixe au robot
			}
			
			//////////////////////////////////////////////////////////////////////////
			//angle optimale en passant les angles sur -PI et PI//////////////////////
			// modulo
			
			angle_restant=Modulo_Pi(angle_restant);// application de la correction
			
			//////////////////////////////////////////////////////////////////////////
			// critères des zones d'arrivées /////////////////////////////////////////

			if(fabs(angle_restant)<(M_PI/15.0)) // arrèt des moteurs quand le robot est à la position Theta demandée
			{
				positionnement_precis_T=  1;
			}
			else
			{
				positionnement_precis_T=0;
			}
			
			if((fabs(distance_restante_reel)<40)) // arrèt des moteurs quand le robot est à la position x et y demandée
			{
				positionnement_precis_U= 1;
			}
			else
			{
				
				positionnement_precis_U=0;
			}
			
			//////////////////////////////////////////////////////////////////////////
					
			break;
		}
		
		
		case TOURNE:
		{
			//////////////////////////////////////////////////////////////////////////
			// Assignation à l'angle ordonné par l'I2C
			
			angle_restant = Ordre_actuel.Theta-position.Theta; // angle à parcourir pour arriver à l'angle désiré sur le repère fixe au robot
			
			
			//////////////////////////////////////////////////////////////////////////
			//angle optimale en passant les angles sur -PI et PI//////////////////////
			// modulo
			
			angle_restant=Modulo_Pi(angle_restant);// application de la correction
			
			//////////////////////////////////////////////////////////////////////////
			// critères des zones d'arrivées /////////////////////////////////////////

			if(fabs(distance_restante)<5) // arrèt des moteurs quand le robot est à la position x et y demandée
			{
				positionnement_precis_U= 1;
			}
			else
			{
				positionnement_precis_U=0;
			}

			if(fabs(angle_restant)<(M_PI/100.0)) // arrèt des moteurs quand le robot est à la position Theta demandée
			{
				positionnement_precis_T= 1;
				positionnement_precis_U= 1;
			}
			else
			{
				positionnement_precis_T=0;
			}
			break;
			//////////////////////////////////////////////////////////////////////////
		}	
#endif		
		default:
			break;
		
	}
	//////////////////////////////////////////////////////////////////////////
}