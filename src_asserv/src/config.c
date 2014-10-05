
#include "../include/config.h"

int init(void)
{
		
	// INTERRUPT
	/* Enable all interrupt levels. */
	PMIC_SetVectorLocationToApplication();
	PMIC_EnableLowLevel();
	PMIC_EnableMediumLevel();
	PMIC_EnableHighLevel();
		
	// CLOCK SYSTEM
	/*  Enable internal 32 MHz ring oscillator and wait until it's
		*  stable. Divide clock by two with the prescaler C and set the
		*  32 MHz ring oscillator as the main clock source. Wait for
		*  user input while the LEDs toggle.
		*/
	CLKSYS_Enable( OSC_RC32MEN_bm );
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );
	do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
		
	pwm_init();
		
	codeurs_init();
		
	/* Set period/TOP value. */
	TC_SetPeriod( &TCD1, 0xFFFF );
		
	/* Select clock source. */
	TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV8_gc );
	TC1_EnableCCChannels( &TCD1, TC1_CCAEN_bm );
	
	TC_SetCompareA( &TCD1, 0x9C40 );
	TC_ForceUpdate(&TCD1);
	TC1_SetCCAIntLevel(&TCD1, TC_CCAINTLVL_LO_gc);
		
	extern void codeurs_init(void);		
		
		
	sei(); //Autorisation des interruptions après l'initialisation des différents coupleurs
	_delay_ms(100);
	
	i2cInit();
	_delay_ms(100);
	i2c_enable();
		
		
		//////////////////////////////////////////////////////////////////////////
	// Test et debuging //////////////////////////////////////////////////////

	//Test en brute force control robot par l'asserv
	
	#if TEST_CODEUSES== 2
		/*int tot_codeur_G = 0;
		int tot_codeur_D = 0;
	
		int tot_codeur_G_2 = 0;
		int tot_codeur_D_2 = 0;*/
	
		//Carré pour test AVANCE /////////////////////////////////////////////////

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


	//Relevé des roues codeuses

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
		
		
		
	return 0;
}
	
	

void Init_codeurs(int *codeur_d,int *codeur_g)
{
	cli(); // Ne pas autoriser les Interuptions
		
	// Valeur du codeur.
	*codeur_d = TCC0.CNT;
	*codeur_g = TCD0.CNT;
		
	// Redemarrage du Timer C0 et D0
	TCC0.CTRLFSET = TC_CMD_RESTART_gc;
	TCC0.CNT = 10000;

	TCD0.CTRLFSET = TC_CMD_RESTART_gc;
	TCD0.CNT = 10000;
		
	sei(); // Réactivation des interruptions
}