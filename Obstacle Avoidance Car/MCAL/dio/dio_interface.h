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
#include "dio_config.h"
#include "dio_private.h"
/* LIB */
#include "../../LIB/std_types/std_types.h"
#include "../../LIB/bit_math/bit_math.h"

/*******************************************************************************************************************************************************************/
/* DIO Functions' Prototypes */
/*******************************************************************************************************************************************************************/
void DIO_init (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Direction_en en_a_direction);
void DIO_write (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Port_value_en en_a_value);
void DIO_read (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,u8 *en_a_data);
void DIO_toggle (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber);
void DIO_setPortDirection( dio_Port_number_en en_a_portNumber,u8 u8_a_PortDirection );
void DIO_setPortValue ( dio_Port_number_en en_a_portNumber,u8 u8_a_PortValue );
void DIO_getPortValue ( dio_Port_number_en en_a_portNumber,u8 *u8_a_PortValue );
void DIO_higher_nipple ( dio_Port_number_en en_a_portNumber,u8 u8_a_data );
void DIO_lower_nipple ( dio_Port_number_en en_a_portNumber,u8 u8_a_data );
/*******************************************************************************************************************************************************************/

#endif /* DIO_INTERFACE_H_ */

