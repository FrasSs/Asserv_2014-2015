 
#include "..\include\Nombre_ordre_enregistre.h"

extern uint8_t new_etat;
extern uint8_t etat;
extern int nb_ordre;

//////////////////////////////////////////////////////////////////////////
// nb_ordre_save : fonction calculant le nombre d'ordre en attente////////
// nb_ordre : nombre d'ordre restant//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void nb_ordre_find(void)
{
	int i,j;
	
	
	if (etat!=new_etat)
	{
		for(i=0,j=etat;j!=new_etat;i++)
		{
			j=((j+1)<STACK_SIZE) ? j+1 : 0;
		}
		
		nb_ordre = i;
	}
	else
	{
		if (nb_ordre)
		{
			nb_ordre=4;
		}
		else
		{
			nb_ordre=0;
		}
	}
}

void nb_ordre_pp(void)
{
	nb_ordre++;
}

void nb_ordre_mm(void)
{
	nb_ordre--;
}