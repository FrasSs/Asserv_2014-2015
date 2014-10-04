
#include "../include/Mouvement_Robot.h"
#include "math.h"
extern int nb_ordre;


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