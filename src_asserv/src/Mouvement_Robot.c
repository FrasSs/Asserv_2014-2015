
#include "../include/Mouvement_Robot.h"

extern double moteur_D;
extern double moteur_G;

int Mouvement_robot(void)
{
	if ((fabs(moteur_D)<=1)||((fabs(moteur_G))<=1))
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}