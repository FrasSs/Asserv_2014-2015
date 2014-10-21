#ifndef ORDRE
#define ORDRE

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