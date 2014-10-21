#ifndef ORDRE
#define ORDRE

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