/*Fichier conçue pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier contenant les prototypes des fonctions pour l'i2c en esclave pour le port E.
//!
//! - Compilateur			: AVR-GCC
//! - Composent supporter	: atxmega32a4
//!
//!
//! \author Maleyrie Antoine
//! \version 0.1
//! \date 12/11/2011
//!
//! ****************************************************************************

//! \defgroup i2c
//! Rapidement :
//! Il faut définir I2C_TWI, dans le ficher user_board.
//!	exemple ->
//! \code
//! #define I2C_TWI TWIC
//! \endcode
//!
//!	En suit il faut définir l'adresse du composent via la define I2C_ADD qui doit ce trouver dans conf_bord.h il faut aussi indiquer la fonction de call back via la define I2C_CALL_BACK.  Et enfin il faut aussi définir la taille des buffers de lecture et d'écriture avec I2C_SIZE_BUFFER_RX et I2C_SIZE_BUFFER_TX .
//!	exemple ->
//! \code
//! #define I2C_ADD 0x30
//! #define I2C_CALL_BACK i2cCallBack
//! #define I2C_SIZE_BUFFER_RX 32
//! #define I2C_SIZE_BUFFER_TX 32
//! \endcode
//!
//! La fonction de call back doit avoirs le prototype suivent :
//! \code
//! void i2cCallBack(uint8_t cmd, uint8_t dataRx[], uint8_t size);
//! \endcode
//!
//! Puis initialiser la bibliothèque via la fonction \ref i2cInit(), vous pouvez utiliser \ref i2cWrite() dans la fonction de call back pour répondre au maître. \n
//!
//!	Un exemple d'implémentation pour la fonction de call back est données si dessous.
//!
//! \code
//!void i2cCallBack(uint8_t cmd, uint8_t dataRx[], uint8_t size)
//!{
//!	static int16_t test = 0;
//!	switch(cmd)
//!	{
//!		case 1:
//!			test++;
//!			i2cWrite((uint8_t *)&test, sizeof test);
//!		break;
//!	}
//!}
//! \endcode
//!
//! @{

#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

//_____ I N C L U D E S ________________________________________________________
#include "twi_slave.h"
//#include "conf_board.h"

//_____ M A C R O S ____________________________________________________________
#define I2C_TWI TWIE
#define I2C_ADD 0x30//'M'
#define I2C_CALL_BACK i2cCallBack

#define I2C_SIZE_BUFFER_RX 32
#define I2C_SIZE_BUFFER_TX 32



// Vérifie l'existiviter de la define définissant le TWI utiliser.
#if !defined(I2C_TWI)
#error I2C_TWI is no defined for i2c.h.
#endif

// Vérifie l'existiviter de la define représentant l'adresse.
#if !defined(I2C_ADD)
#error I2C_ADD is no defined for i2c.h.
#endif

// Vérifie l'existiviter de la fonction de callBack
#if !defined(I2C_CALL_BACK)
#error I2C_CALL_BACK is no defined for i2c.h.
#endif

// Vérifie l'existiviter de la define représentant la taille du buffer de lecture.
#if !defined(I2C_SIZE_BUFFER_RX)
#error I2C_SIZE_BUFFER_RX is no defined for i2c.h.
#endif

// Vérifie l'existiviter de la define représentant la taille du buffer d'écriture.
#if !defined(I2C_SIZE_BUFFER_TX)
#error I2C_SIZE_BUFFER_TX is no defined for i2c.h.
#endif


//_____ P R O T O T Y P E S ____________________________________________________

void i2cInit(void);
void i2cWrite(uint8_t data[], const uint8_t size);

//! \brief Permet d'activer l'i2c (activer à l'initialisation \ref i2cInit()).
static inline void i2c_enable(void)
{
	twi_slave_enable(&I2C_TWI);
}

//! \brief Permet de déactiver l'i2c
static inline void i2c_disable(void)
{
	twi_slave_disable(&I2C_TWI);
}

//! @}

#endif // I2C_H_INCLUDED
