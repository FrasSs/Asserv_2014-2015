

#include "../include/PI_Asservissement.h"
#include "../include/config.h"
//////////////////////////////////////////////////////////////////////////
// fct_PI : calcule de l'asservissement (Régulation PI) //////////////////
//////////////////////////////////////////////////////////////////////////

double fct_PI(int action,double Ki, double seuil_I, double Kp,double Erreur_E,double Te)
{
	static double SumErreurTourne = 0.0;
	static double SumErreurAvance = 0.0;
	
	double * SumErreur;
	
	if(action == TOURNE)
	{
		SumErreur = &SumErreurTourne;
	}
	
	if(action == AVANCE)
	{
		SumErreur = &SumErreurAvance;
	}
	
	
	*SumErreur += Erreur_E;
	
	if(*SumErreur > seuil_I)
	{
		*SumErreur = seuil_I;
	}
	
	if(*SumErreur < -seuil_I)
	{
		*SumErreur = -seuil_I;
	}
	
	double Ptmp = Kp*Erreur_E;
	double Itmp = Ki*(*SumErreur);
	
	return Ptmp + Itmp;
}
	