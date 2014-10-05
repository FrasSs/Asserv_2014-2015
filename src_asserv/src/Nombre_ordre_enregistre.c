 
#include "..\include\Nombre_ordre_enregistre.h"

extern uint8_t new_etat;
extern uint8_t etat;
extern int nb_ordre;

//////////////////////////////////////////////////////////////////////////
// nb_ordre_save : fonction calculant le nombre d'ordre en attente////////
// nb_ordre : nombre d'ordre restant//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void nb_ordre_save(void)
{
	int i=0,j=etat;
	
	for(i=0,j=etat;j!=new_etat;i++)
	{
		j=(j<STACK_SIZE) ? j+1 : 0;
	}
	nb_ordre = i;
}