/*
 * main.c
 *
 * Created: 18/09/2012 16:26:30
 *  Author: Manuel
 *		Note le minimum pour faire tourn� des moteurs.FONCTIONNE
 */ 

#define F_CPU 32000000UL

#include <util/delay.h>
#include <stdint.h>
#include <math.h>
#include "./include/config.h"
#include "./hardware/avr_compiler.h"

//////////////////////////////////////////////////////////////////////////
// I2C ///////////////////////////////////////////////////////////////////

extern uint8_t I2CNewOrderFlag;
Commande I2CNewOrder;

extern int Com_i2c;

//////////////////////////////////////////////////////////////////////////
// Constante de temps ////////////////////////////////////////////////////
volatile static unsigned char goAsserv=0;

//////////////////////////////////////////////////////////////////////////
// Odom�trie /////////////////////////////////////////////////////////////

int codeur_g = 10000; // Variable contr�l� grace au timer dont l'horloge et une roue codeuse
int codeur_d = 10000; // Variable contr�l� grace au timer dont l'horloge et une roue codeuse

Commande position={0,0,0,0}; // Position temps r�el du robot

double distance_Theta = 0.0;// Vitesse R�elle = vitesse_Theta*100(donne des rad/s) car vitesse_Theta et en rad/0.01s.
double distance_U = 0.0;	// Vitesse R�elle = vitesse_U*/10.0	(donne des m/s) car vitesse_U et en mm/0.01s.


//////////////////////////////////////////////////////////////////////////
// Ordres ////////////////////////////////////////////////////////////////

#define STACK_SIZE 4 // taille de la pile utilis�
Commande Ordre1={STOP,0,0,0};
Commande Ordre2={STOP,0,0,0};
Commande Ordre3={STOP,0,0,0};
Commande Ordre4={STOP,0,0,0};
Commande End_ordre={0,0,0,0};
Commande* Tab_ordre[] = {&Ordre1 , &Ordre2, &Ordre3, &Ordre4,&End_ordre}; // Pile d'ordre
Commande Ordre_actuel;	// Ordre en cours
Commande Ordre_suivant; // Ordre suivant

uint8_t etat = 0;		// num�ro de l'ordre en cour
uint8_t new_etat = 0;	// num�ro du dernier ordre re�u
int nb_ordre;			// nombre d'ordre enregistr�

//////////////////////////////////////////////////////////////////////////
// Autres ////////////////////////////////////////////////////////////////
// Vitesse Commande
double Vitesse_C_U = 0.0 ;
double Vitesse_C_T = 0.0 ;

// Trapeze de vitesse
double distance_restante=0.0;
double angle_restant=0.0;

// Ralentissement pour l'action AVANCE_Free
double V_Min=0.0;

//arriv� � l'endoit d�sir�
int positionnement_precis_U=0;
int positionnement_precis_T=0;

// PID 
double PID_V_U=0.0; 
double PID_V_T=0.0;

// Blocage
int BugBloquage=0;

// Moteur
double moteur_G = 0.0;
double moteur_D = 0.0;

//////////////////////////////////////////////////////////////////////////
//Test
#define TEST_CODEUSES 0 

//////////////////////////////////////////////////////////////////////////


int main(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Initialisation ////////////////////////////////////////////////////////
	
	init();
	
	//////////////////////////////////////////////////////////////////////////
	// Test et debuging //////////////////////////////////////////////////////

	//Test en brute force control robot par l'asserv
	
	#if TEST_CODEUSES== 2
	/*int tot_codeur_G = 0;
	int tot_codeur_D = 0;
	
	int tot_codeur_G_2 = 0;
	int tot_codeur_D_2 = 0;*/
	
//Carr� pour test AVANCE /////////////////////////////////////////////////

	Ordre1.Type = AVANCE;
	Ordre1.X = 0.0;
	Ordre1.Y = 0.0;//500.0;
	Ordre1.Theta = -M_PI_2;
		
	Ordre2.Type = AVANCE;
	Ordre2.X = 300.0;
	Ordre2.Y = 500.0;
	Ordre2.Theta = -M_PI;
		
	Ordre3.Type = AVANCE;
	Ordre3.X = 300.0;
	Ordre3.Y = 0.0;
	Ordre3.Theta = M_PI_2;
		
	Ordre4.Type = AVANCE;
	Ordre4.X = 0.0;
	Ordre4.Y = 0.0;
	Ordre4.Theta = 0;
	
	new_etat=4;
	nb_ordre=4;
	
//Arc de cerlce pour tester AVANCE_Free////////////////////////////////////

	/*Ordre1.Type = AVANCE_Free;
	Ordre1.X = 50.0;
	Ordre1.Y = 150.0;
	Ordre1.Theta = -M_PI_2;
	
	Ordre2.Type = AVANCE_Free;
	Ordre2.X = 225.0;
	Ordre2.Y = 330.0;
	Ordre2.Theta = -M_PI;
	
	Ordre3.Type = AVANCE_Free;
	Ordre3.X = 400.0;
	Ordre3.Y = 400.0;
	Ordre3.Theta = M_PI_2;
	
	Ordre4.Type = AVANCE;
	Ordre4.X = 0.0;
	Ordre4.Y = 0.0;
	Ordre4.Theta = 0;
	
///Autre /////////////////////////////////////////////////////////////////
	
	
	Ordre1.Type = AVANCE;
	Ordre1.X = 0.0;
	Ordre1.Y = 600.0;
	Ordre1.Theta = -M_PI_2;
	
	Ordre2.Type = AVANCE;
	Ordre2.X = -900.0;
	Ordre2.Y = 600.0;
	Ordre2.Theta = -M_PI_2;
	
	Ordre3.Type = AVANCE;
	Ordre3.X = -1500.0;
	Ordre3.Y = 600.0;
	Ordre3.Theta = 0;
	
	Ordre4.Type = AVANCE;
	Ordre4.X = -1500.0;
	Ordre4.Y = 0.0;
	Ordre4.Theta = 0;
	
	new_etat=4;
		*/
	#endif	


	//Relev� des roues codeuses

	#if TEST_CODEUSES==1
tot_codeur_D=tot_codeur_D+Sens_codeur_D*(codeur_d - 10000);
tot_codeur_G=tot_codeur_G+Sens_codeur_G*(codeur_g - 10000);
			
			
if(tot_codeur_D>=20000)
{
	tot_codeur_D-=20000;
	tot_codeur_D_2++;
}
if(tot_codeur_D<=-20000)
{
	tot_codeur_D+=20000;
	tot_codeur_D_2--;
}
			
			
if(tot_codeur_G>=20000)
{
	tot_codeur_G-=20000;
	tot_codeur_G_2++;
}
if(tot_codeur_G<=-20000)
{
	tot_codeur_G+=20000;

	tot_codeur_G_2--;
}
	#endif

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	while(1)
	{
		if(goAsserv) // demande de raffraichissement de l'asservissement (toutes des ms)
		{
			//////////////////////////////////////////////////////////////////////////
			// Initialisation ////////////////////////////////////////////////////////
			
			Init_codeurs(&codeur_d,&codeur_g); // initialisation codeuses
			
			
			//////////////////////////////////////////////////////////////////////////
			// ODOMETRIE /////////////////////////////////////////////////////////////
			
			fct_odometrie();
			
			//////////////////////////////////////////////////////////////////////////
			// I2C ///////////////////////////////////////////////////////////////////
			
			if (I2CNewOrderFlag)  // un nouvel ordre recu sur le bus I2C
			{
				// Mise � jour du pointeur de la pile d'ordre pour point� sur le dernier ordre re�u
				Actualisation_new_Ordre(STACK_SIZE); 
				
				I2CNewOrderFlag = 0;
			}
			
			//////////////////////////////////////////////////////////////////////////
			// Ordres ////////////////////////////////////////////////////////////////
			
			switch (Ordre_actuel.Type)
			{
// AVANCE
				case AVANCE :
				{
					position.Type=AVANCE;
					
					//////////////////////////////////////////////////////////////////////////
					// Calcule des distantes restantes � parcourir
						
					Calcule_deplacement(AVANCE); 
					
					//////////////////////////////////////////////////////////////////////////
					// Trap�ze vitesse avance 
					
					Vitesse_C_U = calculTrapez(AVANCE,Vitesse_C_U, 0/*V_Min*/,	70.0/*V_Max mm/10ms */,		distance_restante,	10.0/*A_Desc mm/(10ms)�*/,		10.0/*A_Acc mm/(10ms)�*/,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,Vitesse_C_T, 0/*V_Min*/,	M_PI/*V_Max rad/10ms*/,		angle_restant,		20.0/*A_Desc rad/(10ms)�*/,		20.0/*A_Acc mm/(10ms)� */,	&positionnement_precis_T);
					
					//////////////////////////////////////////////////////////////////////////
					// Asservi PID
					// assignation aux variables des moteurs d'une nouvelle commande
					
					PID_V_U= fct_PI(AVANCE,	Ki_U,	1.0,	Kp_U,	Vitesse_C_U-distance_U,		0.01);
					PID_V_T= fct_PI(TOURNE,	Ki_T,	1.0,	Kp_T,	Vitesse_C_T-distance_Theta,	0.01);
					
					//////////////////////////////////////////////////////////////////////////
					// assignation aux variables des moteurs d'une nouvelle commande
					moteur_G = PID_V_U - PID_V_T ;
					moteur_D = PID_V_U + PID_V_T ;
					
					break;
				}

// >>> AVANCE_Free				
				case AVANCE_Free :
				{
					position.Type=AVANCE_Free;
					//////////////////////////////////////////////////////////////////////////
					// vitesse de n�gociation du changement de cap
					
					V_Min=Calcul_ralentissement(Vitesse_C_U,30.0,position); // ATTETION la valeur V_Min en sortie est non sign�
					
					//////////////////////////////////////////////////////////////////////////
					// Calcule des distantes restantes � parcourir	
					
					Calcule_deplacement(AVANCE_Free); 
					
					//////////////////////////////////////////////////////////////////////////
					// Trap�ze vitesse avance 
					
					Vitesse_C_U = calculTrapez(AVANCE,	Vitesse_C_U,	0/*V_Min*/,		5/*V_Max mm/10ms */,		distance_restante,	2.0/*A_Desc mm/(10ms)� */,	2.0/*A_Acc mm/(10ms)� */,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,	Vitesse_C_T,	0/*V_Min*/,		0.115/*V_Max rad/10ms */,	angle_restant,		3.0/*A_Desc rad/(10ms)� */, 3.0/*A_Acc rad/(10ms)� */,	&positionnement_precis_T);
					
					//////////////////////////////////////////////////////////////////////////
					// Asservi PID
					// assignation aux variables des moteurs d'une nouvelle commande
									
					PID_V_U=fct_PI(AVANCE,	Ki_U,	0 ,	Kp_U,	Vitesse_C_U-distance_U,		0.01);
					PID_V_T=fct_PI(TOURNE,	Ki_T,	1,	Kp_T,	Vitesse_C_T-distance_Theta,	0.01);
					
					//////////////////////////////////////////////////////////////////////////
					// assignation aux variables des moteurs d'une nouvelle commande
					
					moteur_G =PID_V_U - PID_V_T ;
					moteur_D =PID_V_U + PID_V_T ;
					
					break;
				}
				
// TOURNE
				case TOURNE :
				{
					position.Type=TOURNE;
					//////////////////////////////////////////////////////////////////////////
					// Trap�ze vitesse angulaire 
					
					Calcule_deplacement(TOURNE); // conversion position cart�sienne en polaire erreur_U la distance et commande_Theta la distance
					
					Vitesse_C_U = calculTrapez(AVANCE,	Vitesse_C_U,	0.0/*V_Min*/,	15.0/*V_Max mm/10ms */,		distance_restante,	2.0/*A_Desc mm/(10ms)� */,	2.0/*A_Acc mm/(10ms)� */,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,	Vitesse_C_T,	0.0/*V_Min*/,	0.115/*V_Max rad/10ms */,	angle_restant,		3.5/*A_Desc rad/(10ms)� */, 2.5/*A_Acc rad/(10ms)� */,	&positionnement_precis_T);
					
					//////////////////////////////////////////////////////////////////////////
					/// Asservi PID
					// assignation aux variables des moteurs d'une nouvelle commande
					
					PID_V_U= fct_PI(AVANCE,	Ki_U,	1 ,	Kp_U,	Vitesse_C_U-distance_U,		0.01);
					PID_V_T= fct_PI(TOURNE,	Ki_T,	1,	Kp_T,	Vitesse_C_T-distance_Theta,	0.01);
					
					//////////////////////////////////////////////////////////////////////////
					// assignation aux variables des moteurs d'une nouvelle commande
					
					moteur_G = PID_V_U - PID_V_T ;
					moteur_D = PID_V_U + PID_V_T ;	
									
					break;
				}
				
// >>> STOP					
				//Par I2C	
				case STOP :
				{
					position.Type=STOP;
					// Initialisation
					moteur_G = 0 ;
					moteur_D = 0 ;
					Vitesse_C_T=0;
					Vitesse_C_U=0;
					distance_restante=0;
					angle_restant=0;
						
					// passage � l'orde suivant si un nouveau � �t� envoy�
					positionnement_precis_U=1;
					positionnement_precis_T=1;
					break;
				}
				
				
				default:
					break;	
					
			} // end Switch

			//////////////////////////////////////////////////////////////////////////
			// Bloquage 
			
			detecBugBloquage();
			
			//////////////////////////////////////////////////////////////////////////
			
			goAsserv = 0; // fin de mise � jour de l'Asservissement
			
		} // end if goAsserv	
		
		//////////////////////////////////////////////////////////////////////////
		// MOTEUR ////////////////////////////////////////////////////////////////
		// Mise � jour des registres des pwm commandant les moteurs
		
		pwm_set(cablage_mot_D,Sens_mot_D*moteur_D); // attention moteur gauche invers�
		pwm_set(cablage_mot_G,Sens_mot_G*moteur_G);// modifi� pour le petit robot
	
		//////////////////////////////////////////////////////////////////////////
		// fonction de d�roulement de la pile + mise � jour les commandes et nb_ordre
		
		fct_Ordre_suivant(&positionnement_precis_U,&positionnement_precis_T,STACK_SIZE,STOP);
		
		//////////////////////////////////////////////////////////////////////////
	} // end while(1)
	
} // end fct main


//////////////////////////////////////////////////////////////////////////
// Que se passe-t-il quand on d�sactive les interruptions au moment de cette interruption
// Qu'elle sera l'impacte sur le calcule de la vitesse de d�placement (constente de temps plus pr�cis�ment
//  DANS config.c :
//		- Valeur de Compar� : 0x9C40=40000(dec) (TC_SetCompareA( &TCD1, 0x9C40 ); )
//		- Clock : 32Mhz (CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );)
//		- Sensibilit� du Timer : Clock/8=4Mhz (TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV8_gc );)
//	=>(1/4Mhz)*40000=10ms
ISR(TCD1_CCA_vect) // Interruption sur Timer permettant un rebouclage temporis�
{
	TC_Restart( &TCD1 ); // red�marrer le timer
	TC_SetCount(&TCD1,0); // Mettre le Compter A du Timer/Comparateur D1 � 0

	goAsserv = 1; // Demande de mise a jour asservissement
}


