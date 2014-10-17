/*Fichier conçue pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier contenant l'implémentation des fonctions pour l'i2c en esclave pour le port c.
//!
//! - Compilateur			: AVR-GCC
//! - Composent supporter	: atxmega32a4
//!
//!
//! \author Maleyrie Antoine
//! \version 0.2
//! \date 12/11/2011
//!
//! ****************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "i2c.h"
#include "leds.h"

//_____ M A C R O S ____________________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________
#if !defined(__DOXYGEN__)
extern void I2C_CALL_BACK(uint8_t cmd, uint8_t dataRx[], uint8_t size);
#endif

//_____ I M P L E M E N T A T I O N S __________________________________________
#if !defined(__DOXYGEN__)
TWI_Slave_t _twiSlave;

// Fonction appeler à chaque réception d'octet. Utile seulement pour le driver du twi.
void _twiHandler(void)
{
}

#endif

//! \brief Initialise l'i2c.
void i2cInit(void)
{
	twi_slave_options_t twi_slave_options;
	twi_slave_setup(&I2C_TWI, &twi_slave_options, &_twiSlave, &_twiHandler, I2C_ADD, TWI_SLAVE_INTLVL_LO_gc);
}

//! \brief Permet d'écrire des données.
//!
//! \note Cette fonction ne vérifie pas la validité du nombre de données à écrire. Vous devez être sur que le paramètre \p size est inférieur ou égale à la macro \b I2C_SIZE_BUFFER_RX.
//! \param data sont les données à envoyer au maître.
//! \param size est le nombre de données.
void i2cWrite(uint8_t data[], const uint8_t size)
{
	//recopie du buffer pour l'écriture.
	for(uint8_t i = 0; i != size; i++)
		_twiSlave.sendData[i] = data[i];
}

#if !defined(__DOXYGEN__)

// Fonction d'interruption du twi C.
ISR(TWIC_TWIS_vect)
{
	//leds_on(LED4);
	TWI_SlaveInterruptHandler(&_twiSlave);

	//Si on est arrivé a la fin d'une lecture on appelle la fonction de call back.
	if((        _twiSlave.result == TWIS_RESULT_OK) && 
			((~(_twiSlave.interface->SLAVE.STATUS & TWI_SLAVE_DIR_bm)) 
			 ||(_twiSlave.interface->SLAVE.STATUS & TWI_SLAVE_DIR_bm)))
	{
		I2C_CALL_BACK(_twiSlave.receivedData[0], (uint8_t*)(_twiSlave.receivedData+1), _twiSlave.bytesReceived-1);
	}
		
	//leds_off(LED4);
}

// Fonction d'interruption du twi E.

ISR(TWIE_TWIS_vect)
{
	//leds_on(LED4);
	TWI_SlaveInterruptHandler(&_twiSlave);
	
	//Si on est arrivé a la fin d'une lecture on appelle la fonction de call back.
	if((          _twiSlave.result == TWIS_RESULT_OK)  
			&&((~(_twiSlave.interface->SLAVE.STATUS & TWI_SLAVE_DIR_bm)) 
			||   (_twiSlave.interface->SLAVE.STATUS & TWI_SLAVE_DIR_bm)))
	{	
		I2C_CALL_BACK(_twiSlave.receivedData[0], (uint8_t*)(_twiSlave.receivedData+1), _twiSlave.bytesReceived-1);
	}
	//leds_off(LED4);
}
#endif
