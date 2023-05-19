/*
 * dio_interface.h
 *
 *     Created on: MAY 16, 2023
 *         Author: Mohamed Abdelsalam - https://github.com/m3adel
 *    Description: This file contains all Digital Input Output (DIO) functions' prototypes and definitions (Macros) to avoid magic numbers.
 *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 */

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/*******************************************************************************************************************************************************************/
/* DIO Includes */

/* LIB */
#include "../../LIB/std_types/std_types.h"
#include "../../LIB/bit_math/bit_math.h"

/*******************************************************************************************************************************************************************/
/* DIO Macros */

/* Port Numbers */
typedef enum {
	A, B, C, D
} dio_Port_number_en;

/* Pin Numbers */
typedef enum {
	P0,	P1,	P2,	P3,	P4,	P5,	P6,	P7
} dio_Pin_number_en;

/* Pin Directions */
typedef enum {
	IN,	OUT
} dio_Direction_en;

/* Pin Values */
typedef enum {
	LOW, HIGH, INPUT_FLOATING, INPUT_PULLUP_RESISTOR
} dio_Port_value_en;

/*******************************************************************************************************************************************************************/
/* DIO Functions' Prototypes */

void DIO_init (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Direction_en direction);
void DIO_write (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Port_value_en value);
void DIO_read (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,u8 *data);
void DIO_toggle (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber);
void DIO_setPortDirection( dio_Port_number_en portNumber,u8 Port_direction );
void DIO_setPortValue ( dio_Port_number_en portNumber,u8 Port_value );
void DIO_getPortValue	   ( dio_Port_number_en portNumber,u8 *Port_value );
void DIO_higher_nipple ( dio_Port_number_en portNumber,u8 data );
void DIO_lower_nipple ( dio_Port_number_en portNumber,u8 data );
/*******************************************************************************************************************************************************************/

#endif /* DIO_INTERFACE_H_ */