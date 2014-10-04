/*
 * config.c
 *
 * Created: 29/09/2012 17:05:55
 *
 */ 

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