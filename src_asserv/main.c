
#if 1

#include "./include/config.h"

//////////////////////////////////////////////////////////////////////////
// I2C ///////////////////////////////////////////////////////////////////

extern uint8_t I2CNewOrderFlag;
Commande I2CNewOrder;

//////////////////////////////////////////////////////////////////////////
// Constante de temps ////////////////////////////////////////////////////
volatile static unsigned char goAsserv=0;

//////////////////////////////////////////////////////////////////////////
// Odometrie /////////////////////////////////////////////////////////////

int codeur_g = 10000; // Variable contra´le grace au timer dont l'horloge et une roue codeuse
int codeur_d = 10000; // Variable contra´le grace au timer dont l'horloge et une roue codeuse

Commande position={0,0,0,0}; // Position temps reel du robot

double distance_Theta = 0.0;// Vitesse Reelle = vitesse_Theta*100(donne des rad/s) car vitesse_Theta et en rad/0.01s.
double distance_U = 0.0;	// Vitesse Reelle = vitesse_U*/10.0	(donne des m/s) car vitesse_U et en mm/0.01s.


//////////////////////////////////////////////////////////////////////////
// Ordres ////////////////////////////////////////////////////////////////

Commande Ordre1={STOP,0,0,0};
Commande Ordre2={STOP,0,0,0};
Commande Ordre3={STOP,0,0,0};
Commande Ordre4={STOP,0,0,0};
Commande* Tab_ordre[] = {&Ordre1 , &Ordre2, &Ordre3, &Ordre4}; // Pile d'ordre
Commande Ordre_actuel;	// Ordre en cours
Commande Ordre_suivant; // Ordre suivant

uint8_t etat = 0;		// numero de l'ordre en cour
uint8_t new_etat = 0;	// numero du dernier ordre rea§u
int nb_ordre=0;			// nombre d'ordre enregistre

int demarrage=0;

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

//arrive a  l'endoit desire
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
// Test et debuging //////////////////////////////////////////////////////

int tot_codeur_D=0;
int tot_codeur_G=0;
int tot_codeur_D_2=0;
int tot_codeur_G_2=0;

#define SIMU Brute_force

//////////////////////////////////////////////////////////////////////////

#endif



int main(void)
{
	leds_configure();
	leds_on(LED1);

#if 1
	//Test en brute force control robot par l'asserv
	#if (SIMU==Brute_force)	
		//Carre pour test AVANCE /////////////////////////////////////////////////

		Ordre1.Type = AVANCE;
		Ordre1.X = 0.0;
		Ordre1.Y = 500.0;
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
		
		demarrage=1;
		etat=0;
		new_etat=0;
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


#endif
	//////////////////////////////////////////////////////////////////////////
	// Initialisation ////////////////////////////////////////////////////////
	
	init();	
	
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
			// Ordres ////////////////////////////////////////////////////////////////
			switch (Ordre_actuel.Type)
			{
// AVANCE
				case AVANCE :
				{
					position.Type=AVANCE;
					
					//////////////////////////////////////////////////////////////////////////
					// Calcule des distantes restantes a parcourir
						
					Calcule_deplacement(AVANCE); 
					
					//////////////////////////////////////////////////////////////////////////
					// Trapeze vitesse avance 

					Vitesse_C_U = 0;//calculTrapez(AVANCE,	Vitesse_C_U,	15,		40.0,		distance_restante,	10.0,		10.0,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,	Vitesse_C_T,	0,		M_PI,		angle_restant,		20.0,		20.0,	&positionnement_precis_T);

					
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
#if 1			
				case AVANCE_Free :
				{
					position.Type=AVANCE_Free;
					//////////////////////////////////////////////////////////////////////////
					// vitesse de negociation du changement de cap
					
					V_Min=Calcul_ralentissement(30.0,	position); // ATTETION la valeur V_Min en sortie est non signe
					
					//////////////////////////////////////////////////////////////////////////
					// Calcule des distantes restantes a  parcourir	
					
					Calcule_deplacement(AVANCE_Free); 
					
					//////////////////////////////////////////////////////////////////////////
					// Trapa¨ze vitesse avance 
					
					Vitesse_C_U = calculTrapez(AVANCE,	Vitesse_C_U,	V_Min/*V_Min*/,		5/*V_Max mm/10ms */,		distance_restante,	2.0/*A_Desc mm/(10ms)Â² */,	2.0/*A_Acc mm/(10ms)Â² */,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,	Vitesse_C_T,	0/*V_Min*/,			0.115/*V_Max rad/10ms */,	angle_restant,		3.0/*A_Desc rad/(10ms)Â² */, 3.0/*A_Acc rad/(10ms)Â² */,	&positionnement_precis_T);
					
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
					// Trapa¨ze vitesse angulaire 
					
					Calcule_deplacement(TOURNE); // conversion position cartesienne en polaire erreur_U la distance et commande_Theta la distance
					
					Vitesse_C_U = calculTrapez(AVANCE,	Vitesse_C_U,	0.0/*V_Min*/,	15.0/*V_Max mm/10ms */,		distance_restante,	2.0/*A_Desc mm/(10ms)Â² */,	2.0/*A_Acc mm/(10ms)Â² */,	&positionnement_precis_U);
					Vitesse_C_T = calculTrapez(TOURNE,	Vitesse_C_T,	0.0/*V_Min*/,	0.115/*V_Max rad/10ms */,	angle_restant,		3.5/*A_Desc rad/(10ms)Â² */, 2.5/*A_Acc rad/(10ms)Â² */,	&positionnement_precis_T);
					
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
						
					// passage a  l'orde suivant si un nouveau a  ete envoye
					positionnement_precis_U=1;
					positionnement_precis_T=1;
					break;
				}
				
				
#endif			
	
				default:
					break;	
					
			} // end Switch

			//////////////////////////////////////////////////////////////////////////
			// Bloquage 
			
			detecBugBloquage();
			
			//////////////////////////////////////////////////////////////////////////
			
			goAsserv = 0; // fin de mise a  jour de l'Asservissement
			
		} // end if goAsserv	
		
		//////////////////////////////////////////////////////////////////////////
		/// PRODUCTEUR I2C ///////////////////////////////////////////////////////
		
		Recep_Ordre_i2c();
		
		// CONSOMMATEUR //////////////////////////////////////////////////////////
		
		Ordre_suivant_fct(&positionnement_precis_U,&positionnement_precis_T,&demarrage); // fonction de déroulement de la pile + mise à jour les commandes et nb_ordre
		
		//////////////////////////////////////////////////////////////////////////
		// MOTEUR ////////////////////////////////////////////////////////////////
		// Mise a  jour des registres des pwm commandant les moteurs

		pwm_set(cablage_mot_D,Sens_mot_D*moteur_D); // attention moteur gauche inversé
		pwm_set(cablage_mot_G,Sens_mot_G*moteur_G); // modifié pour le petit robot

		//////////////////////////////////////////////////////////////////////////
		
		
			//Releve des roues codeuses
			#if (SIMU==Test_codeuse)
			
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
		
		
	} // end while(1)
	
} // end fct main


//////////////////////////////////////////////////////////////////////////
// Que se passe-t-il quand on desactive les interruptions au moment de cette interruption
// Qu'elle sera l'impacte sur le calcule de la vitesse de deplacement (constente de temps plus precisement
//  DANS config.c :
//		- Valeur de Compare : 0x9C40=40000(dec) (TC_SetCompareA( &TCD1, 0x9C40 ); )
//		- Clock : 32Mhz (CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );)
//		- Sensibilite du Timer : Clock/8=4Mhz (TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV8_gc );)
//	=>(1/4Mhz)*40000=10ms
ISR(TCD1_CCA_vect) // Interruption sur Timer permettant un rebouclage temporise
{
	TC_Restart( &TCD1 ); // redemarrer le timer
	TC_SetCount(&TCD1,0); // Mettre le Compter A du Timer/Comparateur D1 a  0

	goAsserv = 1; // Demande de mise a jour asservissement
}


