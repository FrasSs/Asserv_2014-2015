/*
 * CFile1.c
 *
 * Created: 27/09/2012 19:33:14
 *  Author : Léon
 *	Corrigé : François
 */ 


#include <math.h>

#include "../include/Lecture_trapeze.h"
#include "../include/config.h"

/*
 * gestionTrajectoire.c
 *
 *  Created on: 23 sept. 2012
 *      Author: Manuel
 */


//////////////////////////////////////////////////////////////////////////
// Trapèze Vitesse                      ///V_Max vitesse maximale/////////
//V_Max_| 1	 ________2________	3		///V_Min vitesse min pour le positionnement précis
//		|	/                :\			///A_Acc Accélération démarrage///
//V_Min_|  /A_Acc            : \A_Desc	///A_Desc déscélération///////////
//		| /                  :  \     	///D_Desc distance estimé pour passer de la vitesse iniciale à V_Min
//	    |X-------------------:---X-----	///D_Prec distance pour le positionnement précis
//							 |   |		//////////////////////////////////
//						    /D_Desc\	//////////////////////////////////
double calculTrapez (int action,double Vitesse,double V_Min,double V_Max, double Dist, double A_Desc,double A_Acc, int *Arrive)
{
	
//////////////////////////////////////////////////////////////////////////
// Variable Local
	int rebouclage = 1; // variable de recouclage

// Pointeur
	double *Vitesse_old;
	double *Dist_old;
	double *D_Desc_old;
	double *D_Desc;
	double *Var;
	int *sens; // avance,recule,stop en fct de Dist
	int *etape; // Etape 1 à 3 : Accélère, même vitesse et descélération
	
	
// Variable Static ///////////////////////////////////////////////////////
// Variable de la fonction appelé par AVANCE -> U	
	static double Vitesse_old_U=0.0;
	static double Dist_old_U=0.0;
	static double D_Desc_old_U=0;
	static double D_Desc_U = 0.0;
	static double Var_U=0.0;
	static int sens_U=0;
	static int etape_U=1;
	
// Variable Static ///////////////////////////////////////////////////////
// Variable de la fonction appelé par AVANCE_Free -> U	
	static double Vitesse_old_UF=0.0;
	static double Dist_old_UF=0.0;
	static double D_Desc_old_UF=0;
	static double D_Desc_UF = 0.0;
	static double Var_UF=0.0;
	static int sens_UF=0;
	static int etape_UF=1;

// Variable de la fonction appelé par Tourne -> T
	static double Vitesse_old_T=0.0;
	static double Dist_old_T=0.0;
	static double D_Desc_old_T=0;
	static double D_Desc_T=0.0;
	static double Var_T=0.0 ;
	static int sens_T=0;
	static int etape_T=1;

//////////////////////////////////////////////////////////////////////////
// mise à jour des variables locales en pointant sur les statics consernés
	if (action==TOURNE) // pour la fct avec l'arg TOURNE
	{
		D_Desc=&D_Desc_T;
		Var=&Var_T;
		etape =&etape_T;
		sens=&sens_T;
		Vitesse_old=&Vitesse_old_T;
		Dist_old=&Dist_old_T;
		D_Desc_old=&D_Desc_old_T;
		Dist=Dist*(ANTRAXE_D+ANTRAXE_G)/2; // passage de la distance angulaire en distance 
	}
	
	if (action==AVANCE) // pour la fct avec l'arg AVANCE
	{
		D_Desc=&D_Desc_U;
		Var=&Var_U;
		etape =&etape_U;
		sens=&sens_U;
		Vitesse_old=&Vitesse_old_U;
		Dist_old=&Dist_old_U;
		D_Desc_old=&D_Desc_old_U;
	}
	
	if (action==AVANCE_Free) // pour la fct avec l'arg AVANCE_Free
	{
		D_Desc=&D_Desc_UF;
		Var=&Var_UF;
		etape =&etape_UF;
		sens=&sens_UF;
		Vitesse_old=&Vitesse_old_UF;
		Dist_old=&Dist_old_UF;
		D_Desc_old=&D_Desc_old_UF;
	}
	
	if (!(*Arrive))
	{
		//////////////////////////////////////////////////////////////////////////
//>>>>>>>>>>>>>>>>> CEtte partie est peut'être source de bug !!!!!!!!!!///
		//////////////////////////////////////////////////////////////////////////
		if (Dist!=0) // si un déplacement est requis
		{
			if(Dist>0) // distinction entre avance et recule
			{
				if (Vitesse<0) // si la vitesse actuelle n'est pas du même sens que le déplacement 
				{
					// ATTENTION non linéraire
					Vitesse=0; // inicialisation de la vitesse
				
				
					//////////////////////////////////////////////////////////////////////////
					// reset complet des variables statices
					*etape=1; // Inicialisation (mettre la variable pointé à un pour recommencer le sycle au démarage)
				}
				*sens=1; // sens du déplacement (avance ou tourne à guauche)
			}
			else
			{
				if (Vitesse>0) // si la vitesse actuelle n'est pas du même sens que le déplacement 
				{
					// ATTENTION non linéraire
					Vitesse=0; // inicialisation de la vitesse
				
				
					//////////////////////////////////////////////////////////////////////////
					// reset complet des variables statices
					*etape=1; // Inicialisation (mettre la variable pointé à un pour recommencer le sycle au démarage)
				}
				*sens=-1; // sens du déplacement (recule ou tourne à droite)
			}
		}
		else
		{
			*sens=0; // sens du déplacement : arrêt
		}
	
		V_Min*=(*sens); // Application du changement (rend signé la valeur de V_Min)
	
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
	
		while(rebouclage)
		{
			rebouclage=0; // éviter la boucle infini et permettre de faire un rebouclage une fois les calcules de 1 effectué,ou la condition de 3 vérifié

			switch (*etape) // lecture du graphe du trapèze
			{
				//////////////////////////////////////////////////////////////////////////
				// Accélération
				case 1 : 
				{	
					//////////////////////////////////////////////////////////////////////////
					//Calcule de la distance de descélération/////////////////////////////////
					//////////////////////////////////////////////////////////////////////////
					// en AVANCE
					if (Vitesse>=V_Min)
					{
						*D_Desc_old=*D_Desc; // sauvegarde de l'ancienne distance de descélération
						*Var=-((fabs(V_Min))-(fabs(Vitesse+(A_Acc))))/(A_Desc); // variable pour le calcule de D_Desc
						*D_Desc=((-A_Desc/2)*(*Var)*(*Var))+((fabs(Vitesse+(A_Acc)))*(*Var)); // mesure de la distance de déscélération nécessaire
					}
					else
					{
						if (!(Vitesse<=V_Min))
						{
							*D_Desc=0;
						}
					}
					
					// en RECULE
					if (Vitesse<=V_Min)
					{
						*D_Desc_old=*D_Desc; // sauvegarde de l'ancienne distance de descélération
						*Var=-((fabs(V_Min))-(fabs(Vitesse+(*sens*A_Acc))))/(A_Desc); // variable pour le calcule de D_Desc
						*D_Desc=((-A_Desc/2)*(*Var)*(*Var))+((fabs(Vitesse+(*sens*A_Acc)))*(*Var)); // mesure de la distance de déscélération nécessaire
					}
					else
					{
						if (!(Vitesse>=V_Min))
						{
							*D_Desc=0;
						}
					}
					//////////////////////////////////////////////////////////////////////////
					
						
					if( (fabs(Dist)>=*D_Desc) && (fabs(Vitesse)<V_Max) ) // si on ne dépasse pas VMAx ou si on as encore assès de place pour ralentir une fois appliqué
					{
						Vitesse+=((*sens)*A_Acc);	// calcule accélération
					}
					else
					{
						(*etape)++; // passage à l'etape suivante
						rebouclage=1; // reboucler directement par le While(recbouclage) de cette fct (ne passe donc pas par le main)
					
						if(fabs(Vitesse)<V_Max) // si la vitesse max n'a pas été atteinte
						{
							*D_Desc=*D_Desc_old; // recuperation de l'ancienne Distance de descélération car elle à été calculé pour le moment n+1 alors que l'on est à n
						}
					}		
				
					break;
				}
			
				//////////////////////////////////////////////////////////////////////////
				// calcul du seuil de descélération et maintien de la vitesse en attendant
				// la nouvelle étape
				case 2 :
 				{
					if((fabs(Dist)<(*D_Desc))) // si on est arrivé ou si la distance restante est inferieur à la distance de descélération.
					{
						(*etape)++;  // passer à l'étape suivante
						rebouclage=1; // reboucler directement par le While(recboucle) de cette fct (ne passe donc pas par le main)
					
						*Dist_old=Dist; // sauvegarde de la distance restante pour le calcule de la vitesse de descélération
						*Vitesse_old=Vitesse; //Idem, sauvegarde de Vitesse
						
						// si la distance est trop faible pour bouger (distance trop courte)
						if (fabs(Vitesse)<V_Min) 
						{
							Vitesse=V_Min;
						}
					}
					else
					{
						*etape=1;
					}
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// descélération 	
				case 3 :  
				{
					Vitesse=((fabs(Dist)/(*Dist_old))*((*Vitesse_old-A_Desc)-(*sens*7)-V_Min))+(*sens*7)+V_Min;
					*etape=1;
					
					break;
				}
				
				default:
					break;
			}
		}
	}
	else // Le robot est arrivé à destination
	{
		if (action==AVANCE_Free)
		{
			Vitesse = V_Min; // arrêt du Robot
		}
		else
		{
			Vitesse = 0.0; // arrêt du Robot
		}
					
		//////////////////////////////////////////////////////////////////////////
		// reset complet des variables statices
		*etape=1; // Inicialisation (mettre la variable pointé à un pour recommencer le sycle au démarage)
		*D_Desc=0;
		*Var=0;
		*sens=0;
		*Vitesse_old=0;
		*Dist_old=0;
		*D_Desc_old=0;
		//////////////////////////////////////////////////////////////////////////
	}
	return Vitesse;
}