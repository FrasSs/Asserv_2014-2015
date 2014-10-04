
#include "../include/Modulo_pi.h"


double Modulo_Pi(double Theta)
{
	if (Theta>=M_PI)
	{
		Theta-=(2*M_PI);
	}

	if (Theta<=(-M_PI))
	{
		Theta+=(2*M_PI);
	}
	return Theta;
}