
//*****************************************************************************
// PIC_Asser_Moteur
// traitementI2C.h
// dsPIC 30F
// V.MATOUILLOT 26/01/08
//*****************************************************************************
#include <math.h>
#include "../include/traitementI2C.h"
#include "../hardware/i2c.h"
#include <avr/io.h>
#include "../hardware/leds.h"
#include "../include/struct_Commande.h"


#include "../hardware/i2c.h"
//== PUBLIC VARIABLES =========================================================
uint8_t I2CNewOrderFlag = 0;
uint8_t I2CNewOrderAkwnoledged = 1;
extern Commande I2CNewOrder;

extern Commande position;
extern int nb_ordre;

// BUFFER ( 20 octets ) : Reception
unsigned char msg[20];
unsigned char* pmsg = & msg[0];
unsigned char msg_size;

// BUFFER ( 20 octets ) : Emission
unsigned char buffer_OUT[20];
unsigned char buffer_OUT_INDEX;
int buffer_OUT_size;

unsigned char ERREURI2C=0;
unsigned int  seuil_blockage=0;

void i2cCallBack(uint8_t cmd, uint8_t dataRx[], uint8_t size)
{
	traitementI2C(--dataRx, ++size);
}

void traitementI2C(unsigned char * msg,unsigned char size)
{
	int eff_ordre=0;
	long temp;
		
	//////////////////////////////////////////////////////////////////////////
	// Read I2C
				
	temp = ((long)((unsigned char)msg[0]));
	I2CNewOrder.Type=(uint8_t)temp;
	switch (I2CNewOrder.Type)
	{
		case POSITION_R :
		{
			//////////////////////////////////////////////////////////////////////////
			// Write i2C

			//temp = 0.5 * COEF;
			temp = (long)(position.X * 10);
			buffer_OUT[3] = (unsigned char)(temp>>24);
			buffer_OUT[2] = (unsigned char)(temp>>16);
			buffer_OUT[1] = (unsigned char)(temp>>8);
			buffer_OUT[0] = (unsigned char)temp;


			//Envoi de theta en dixieme de miliRadians(1 = 0.0001 radians)
			//temp = 0.1 * COEF;
			temp = (long)(position.Y * 10);
			buffer_OUT[7] = (unsigned char)(temp>>24);
			buffer_OUT[6] = (unsigned char)(temp>>16);
			buffer_OUT[5] = (unsigned char)(temp>>8);
			buffer_OUT[4] = (unsigned char)temp;
				
			//Envoi de theta en dixieme de miliRadians(1 = 0.0001 radians)
			//temp = 0.1 * COEF;
			temp = (long)(position.Theta * 10000);
			buffer_OUT[11] = (unsigned char)(temp>>24);
			buffer_OUT[10] = (unsigned char)(temp>>16);
			buffer_OUT[9] = (unsigned char)(temp>>8);
			buffer_OUT[8] = (unsigned char)temp;
				
			i2cWrite(buffer_OUT, 12);
			I2CNewOrderFlag = 0;
			break;
		}
			
		case STATUS_ROB :
		{
			if (!Mouvement_robot())
			{
				buffer_OUT[0] = Ordre_actuel.Type;
			}
			else
			{
				buffer_OUT[0] = STOP;
			}
				
			i2cWrite(buffer_OUT, 1);
			I2CNewOrderFlag = 0;
			break;
		}
			
		case STOP_F:
		{
			fct_STOP_F(STOP);
			I2CNewOrder.Type=STOP;
			I2CNewOrderFlag = 1;
			break;
		}
			
		case POSITION_W:
		{
			temp = ((long)((unsigned char)msg[4]))<<24;
			temp += ((long)((unsigned char)msg[3]))<<16;
			temp += ((long)((unsigned char)msg[2]))<<8;
			temp += ((long)((unsigned char)msg[1]));
			position.X=((double)temp / (double)10);

			//Reception de Y en dixieme de mm (1 = 0.0001 metre)
			temp = ((long)((unsigned char)msg[8]))<<24;
			temp += ((long)((unsigned char)msg[7]))<<16;
			temp += ((long)((unsigned char)msg[6]))<<8;
			temp += ((long)((unsigned char)msg[5]));
			position.Y=((double)temp / (double)10);

			//Reception de theta en dixieme de miliRadians(1 = 0.0001 radians)
			temp = ((long)((unsigned char)msg[12]))<<24;
			temp += ((long)((unsigned char)msg[11]))<<16;
			temp += ((long)((unsigned char)msg[10]))<<8;
			temp += ((long)((unsigned char)msg[9]));
			position.Theta=((double)temp / (double)10000);
				
			for(eff_ordre=0;Tab_ordre[eff_ordre+1]->Type==0;eff_ordre++) // effassement de tous les ordres
			{
				Tab_ordre[eff_ordre]->Type=STOP;
				Tab_ordre[eff_ordre]->X=0;
				Tab_ordre[eff_ordre]->Y=0;
				Tab_ordre[eff_ordre]->Theta=0;
			}
			I2CNewOrderFlag = 1;
			break;
		}
			
			
		case AVANCE:
		{
			temp = ((long)((unsigned char)msg[4]))<<24;
			temp += ((long)((unsigned char)msg[3]))<<16;
			temp += ((long)((unsigned char)msg[2]))<<8;
			temp += ((long)((unsigned char)msg[1]));
			I2CNewOrder.X=((double)temp / (double)10);

			//Reception de Y en dixieme de mm (1 = 0.0001 metre)
			temp = ((long)((unsigned char)msg[8]))<<24;
			temp += ((long)((unsigned char)msg[7]))<<16;
			temp += ((long)((unsigned char)msg[6]))<<8;
			temp += ((long)((unsigned char)msg[5]));
			I2CNewOrder.Y=((double)temp / (double)10);

			//Reception de theta en dixieme de miliRadians(1 = 0.0001 radians)
			temp = ((long)((unsigned char)msg[12]))<<24;
			temp += ((long)((unsigned char)msg[11]))<<16;
			temp += ((long)((unsigned char)msg[10]))<<8;
			temp += ((long)((unsigned char)msg[9]));
			I2CNewOrder.Theta=((double)temp / (double)10000.0);
				
			I2CNewOrderFlag = 1;
			break;
		}
		
		default:
			break;
	}
}
