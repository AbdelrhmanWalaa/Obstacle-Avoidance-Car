/*
 * dio_interface.h
 *
 *     Created on: Mar 31, 2021
 *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
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

/* The 4 Ports counted from 0 to 3 */
#define DIO_U8_PORTA		0
#define DIO_U8_PORTB		1
#define DIO_U8_PORTC		2
#define DIO_U8_PORTD		3

/* Port direction as input = 0 or output = 255 */
#define DIO_U8_PORT_INPUT	0
#define DIO_U8_PORT_OUTPUT	255

/* Port value as low = 0 or high = 255 */
#define DIO_U8_PORT_LOW		0
#define DIO_U8_PORT_HIGH	255

/* The 8 Pins counted from 0 to 7 */
#define DIO_U8_PIN0			0
#define DIO_U8_PIN1			1
#define DIO_U8_PIN2			2
#define DIO_U8_PIN3			3
#define DIO_U8_PIN4			4
#define DIO_U8_PIN5			5
#define DIO_U8_PIN6			6
#define DIO_U8_PIN7			7

/* Pin direction as input = 0 or output = 1 */
#define DIO_U8_PIN_INPUT	0
#define DIO_U8_PIN_OUTPUT	1

/* Pin value as low = 0 or high = 1 */
#define DIO_U8_PIN_LOW		0
#define DIO_U8_PIN_HIGH		1

/*******************************************************************************************************************************************************************/
/* DIO Functions' Prototypes */

vd DIO_initialization  ( void );

u8 DIO_setPinDirection ( u8 u8_a_portId, u8 u8_a_pinId, u8 u8_a_pinDirection );
u8 DIO_setPinValue     ( u8 u8_a_portId, u8 u8_a_pinId, u8 u8_a_pinValue );
u8 DIO_getPinValue 	   ( u8 u8_a_portId, u8 u8_a_pinId, u8 *pu8_a_returnedPinValue );
u8 DIO_togglePinValue  ( u8 u8_a_portId, u8 u8_a_pinId );

u8 DIO_setPortDirection( u8 u8_a_portId, u8 u8_a_portDirection );
u8 DIO_setPortValue	   ( u8 u8_a_portId, u8 u8_a_portValue );
u8 DIO_getPortValue	   ( u8 u8_a_portId, u8 *pu8_a_returnedPortValue );
u8 DIO_togglePortValue ( u8 u8_a_portId );

/*******************************************************************************************************************************************************************/

#endif /* DIO_INTERFACE_H_ */