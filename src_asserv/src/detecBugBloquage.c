
#include "../include/detecBugBloquage.h"
#include "../include/STOP_Force.h"

#define Seuil_Theta 0
#define Seuil_U 0

//////////////////////////////////////////////////////////////////////////
// extern Variable
	
extern Commande position;
extern double PID_V_T;
extern double PID_V_U;
extern int positionnement_precis_U;
extern int positionnement_precis_T;
extern int BugBloquage;
extern Commande Ordre_actuel;
extern double moteur_G; // variable Commande moteur
extern double moteur_D; // variable Commande moteur
	
//////////////////////////////////////////////////////////////////////////
//detectBugBloquage : fonction qui à pour but de capter les bug///////////
/////////////////////et les blocages//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	
void detecBugBloquage()
{
	// cercle U
	static int capteur_bug_Theta=0;
	static int capteur_bug_U=0;
	
	static double point_milieu_X=-500;
	static double point_milieu_Y=-500;
	double rayon=PID_V_U*PID_V_U*0.5;
	static int sens_avance_old=0;	
	
	// cercle Theta
	static double point_milieu_Theta=-500;
	double detla_theta=PID_V_T*PID_V_T*0.0005;
	static int sens_rot_old=0;	
	
	// Inicialisation cercles
	if ((point_milieu_X==-500)&&(point_milieu_Y==-500))
	{// initialisation debut 
		point_milieu_X=position.X;
		point_milieu_Y=position.Y;
		point_milieu_Theta=position.Theta;
		sens_rot_old=(PID_V_T>0)? 1 : -1;
		sens_avance_old=(PID_V_U>0)? 1 : -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// Bug U 
	
	// condition sur cercle pour position
	if (((((position.X-point_milieu_X)*(position.X-point_milieu_X))+
		  ((position.Y-point_milieu_Y)*(position.Y-point_milieu_Y)))<(rayon)) &&
		  (positionnement_precis_U!=1))
	{
		capteur_bug_U++;
	}
	else
	{
		point_milieu_X=position.X;
		point_milieu_Y=position.Y;
		capteur_bug_U=0;
	}
	
	
	// Changement de sens de déplacement
	if (!(((PID_V_U>=Seuil_U)&&(sens_avance_old>0))||((PID_V_U<-Seuil_U)&&(sens_avance_old<0))))
	{// si changement de sens de l'avance
		capteur_bug_U=0;
		sens_avance_old=(PID_V_U>0)? 1 : -1;
	}
	
	
// 	if (capteur_bug_U>20)
// 	{
// 		BugBloquage=1;
// 	}
// 	else
// 	{
// 		BugBloquage=0;
// 	}

	//////////////////////////////////////////////////////////////////////////
	// Bug Theta
	
	if ((fabs(position.Theta-point_milieu_Theta)<detla_theta)&&(positionnement_precis_T!=1))
	{
		capteur_bug_Theta++;
	}
	else
	{
		point_milieu_Theta=position.Theta;
		capteur_bug_Theta=0;
	}
	
	
	// Changement de sens de rotation
	if (!(((PID_V_T>=Seuil_Theta)&&(sens_rot_old>0))||((PID_V_T<-Seuil_Theta)&&(sens_rot_old<0))))
	{// si changement de sens de rotation
		capteur_bug_Theta=0;
		sens_rot_old=(PID_V_T>=0)? 1 : -1;
	}
	
	// Bocage

// 	if (capteur_bug_Theta>50)
// 	{
// 		BugBloquage=1;
// 	}
// 	else
// 	{
// 		BugBloquage=0;
// 	}
	
	//////////////////////////////////////////////////////////////////////////

	// initialisation
	if (BugBloquage)
	{
		capteur_bug_Theta=0;
		capteur_bug_U=0;
		
		point_milieu_X=-500;
		point_milieu_Y=-500;
		
		point_milieu_Theta=-500;
	}
	
	if (BugBloquage) // Blocage ou bug capté
	{
		moteur_D=0;
		moteur_G=0;
		
		fct_STOP_F();
		position.Type=BLOQUE;
		Ordre_actuel.Type=BLOQUE; // forcer le blocage
	}
}

//////////////////////////////////////////////////////////////////////////
// acknowledge_bloquage : fonction qui remet l'état de BugBloquage à 0////
//////////////////////////////////////////////////////////////////////////

void acknowledge_bloquage() 
{
	extern int BugBloquage;
	BugBloquage=0;	
}