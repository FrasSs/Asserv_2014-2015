/*
 * config.c
 *
 * Created: 29/09/2012 17:05:55
 *
 */ 

#include "../include/Odometrie.h"

//////////////////////////////////////////////////////////////////////////
// Repère absolu :			^ Y		
//							|	
//							|		
//							|			
//							|						X
//	 -----------------------+----------------------->
//

//////////////////////////////////////////////////////////////////////////
// Variable extern 
extern int codeur_d;
extern int codeur_g;
extern double distance_U; // mm ou vitesse mm/10ms
extern double distance_Theta; // rad ou vitesse rad/10ms
extern Commande position;
//////////////////////////////////////////////////////////////////////////

void fct_odometrie()
{	
	double codeur_d_courant,codeur_g_courant,Rayon_rotation,corde,angle_projection,relatif_X,relatif_Y;
		
	//////////////////////////////////////////////////////////////////////////
	// Mise à l'échelle des résultats des codeuses => rendre ce résultat signé
	// en fct du sens de rotation ////////////////////////////////////////////
	// Le résultat de l'oppération donne la distance parcourue (mm) //////////
	
	codeur_d_courant = Sens_codeur_D*(codeur_d - 10000)*INC_CODEUR_D; // pas en mm
	codeur_g_courant = Sens_codeur_G*(codeur_g - 10000)*INC_CODEUR_G; // pas en mm
	
	//////////////////////////////////////////////////////////////////////////
	// Vitesse 
	
	// distance parcouru en mm ou vitesse mm/10ms
	distance_U = (codeur_g_courant*(ANTRAXE_D/(ANTRAXE_D+ANTRAXE_G)))+(codeur_d_courant*(ANTRAXE_G/(ANTRAXE_D+ANTRAXE_G)));  
	// angle parcouru en radian ou vitesse rad/10ms
	distance_Theta = ((codeur_d_courant-codeur_g_courant)/(ANTRAXE_D+ANTRAXE_G));
	
	//////////////////////////////////////////////////////////////////////////
	
			
	if(distance_Theta>=((5000*M_PI)/(100000*180))) //seuil de la ligne droite
	{
		//////////////////////////////////////////////////////////////////////////
		// cas ou il y a un arc de cercle prononcé ///////////////////////////////
		
		Rayon_rotation=distance_U/(distance_Theta); // rayon du cercle tracé par le déplacement en mm
		corde=2*Rayon_rotation*sin(distance_Theta/2); // calcule de la corde du l'arc de cercle au centre du robot en mm
		angle_projection=(distance_Theta/2)+position.Theta; // angle permettant la projection de la corde sur le repère fixe au sol, en radian
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// cas ou on fais l'aproximation que la trajectoire à effectuer est //////
		// purement une droite la corde et la distance parcourue sont les même ///
		
		angle_projection=(distance_Theta/2)+position.Theta; // angle permettant la projection de la corde sur le repère fixe au sol, en radian
		corde=distance_U;
		
		//////////////////////////////////////////////////////////////////////////
	}
	
	// déplacement relatif en mm
	relatif_X=-sin(angle_projection)*corde;
	relatif_Y=cos(angle_projection)*corde;
	
	//////////////////////////////////////////////////////////////////////////
	// position absolu
	position.Theta= fmod(position.Theta + distance_Theta,2*M_PI); // position précédente à laquelle on ajoute la distance parcouru
	
	position.X = position.X + relatif_X;
	position.Y = position.Y + relatif_Y;
	//////////////////////////////////////////////////////////////////////////
}


